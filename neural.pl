#!/usr/bin/perl

use strict;
use warnings;

use constant GENERATIONS => 5000;

##################################################

# Create the network
print "Generate network...\n";
my $network = new Network();
$network->addLayer(2);
$network->addLayer(4);
$network->addLayer(1);
$network->solidify();


# Train the network
printf("Training network with %d generations...\n", GENERATIONS);
$network->addTrainingData([1, 1], [0]);
$network->addTrainingData([1, 0], [1]);
$network->addTrainingData([0, 1], [1]);
$network->addTrainingData([0, 0], [0]);

$network->train(GENERATIONS);

## Test the network
print "XOR output test:\n";
print "1 xor 1 = " . sprintf("%.2f", ($network->execute([1, 1]))[0]) . "\n";
print "1 xor 0 = " . sprintf("%.2f", ($network->execute([1, 0]))[0]) . "\n";
print "0 xor 1 = " . sprintf("%.2f", ($network->execute([0, 1]))[0]) . "\n";
print "0 xor 0 = " . sprintf("%.2f", ($network->execute([0, 0]))[0]) . "\n";

##################################################

package Node;

sub new {
    my ($class, $id) = @_;
    my $ref = {
        id => $id,
        input_references => [],
        output_references => [],
        input_values => {},
        error => 0,
        output => 0,
        delta => {},
    };

    bless $ref, $class;
}

sub id {
    my ($self) = @_;
    return $self->{"id"};
}

sub addInputEdge {
    my ($self, $node, $weight) = @_;
    $weight = rand() - 0.5 if not defined($weight);

    push(@{$self->{"input_references"}}, [ $node, $weight ]);
}

sub clearInputEdges {
    my ($self) = @_;
    $self->{"input_references"} = [];
}

sub addOutputEdge {
    my ($self, $node, $weight) = @_;
    push(@{$self->{"output_references"}}, $node);
}

sub addWeightDelta {
    my ($self, $node_id, $delta) = @_;
    $self->{"delta"}->{$node_id} += $delta;
}

sub getWeightDelta {
    my ($self, $node_id) = @_;
    return $self->{"delta"}->{$node_id};
}

sub clearWeightDelta {
    my ($self) = @_;
    $self->{"delta"} = {};
}

sub setInputValue {
    my ($self, $node_id, $value) = @_;
    if (!defined($node_id)) { $node_id = "undef" };
    $self->{"input_values"}->{$node_id} = $value;
}

sub getInputValue {
    my ($self, $node_id) = @_;
    if (!defined($node_id)) { $node_id = "undef" };
    return $self->{"input_values"}->{$node_id};
}

sub getOutputNodes {
    my ($self) = @_;
    return @{$self->{"output_references"}};
}

sub getInputEdges {
    my ($self, $node_id) = @_;
    return @{$self->{"input_references"}};
}


sub validate {
    # make sure there is an input value for each input node
    my ($self) = @_;

    for my $tuple (@{$self->{"input_references"}}) {
        my ($node, $weight) = @$tuple;
        my $value = $self->getInputValue($node ? $node->id : undef);

        if (!defined($value)) {
            die sprintf(
                "No input value for node %s",
                $node ? $node->id : "undef"
            );
        }
    }
}

sub execute {
    my ($self) = @_;
    $self->validate();

    # Go through each input value, multiply by the weight, and accumulate
    my $accumulator = 0;
    for my $tuple (@{$self->{"input_references"}}) {
        my ($node, $weight) = @$tuple;
        my $value = $self->getInputValue($node ? $node->id : undef);

        $accumulator += ($value * $weight);
    }

    # Clear out error values
    $self->clear();

    # Return a value
    my $output =  $self->sigmoid($accumulator);
    $self->{"output"} = $output;
    return $output;
}

sub addError {
    my ($self, $error) = @_;
    $self->{"error"} += $error;
}

sub clear {
    my ($self) = @_;
    $self->{"error"} = 0;
    $self->{"output"} = 0;
}

sub getOutputValue {
    my ($self) = @_;
    return $self->{"output"};
}

sub getError {
    my ($self) = @_;
    return $self->{"error"};
}

sub sigmoid {
    my ($self, $x) = @_;
    my $y = 1 / (1 + 2.718 ** (-10 * $x));
    return $y;
}

sub print {
    my ($self) = @_;
    printf ("  > Node %d\n", $self->id);

    # Input nodes
    my $in = $self->{"input_references"};
    for (@$in) {
        my ($node, $weight) = @$_;

        # Get displayable value
        my $value = $self->getInputValue($node ? $node->id : undef);
        if (defined($value)) {
            $value = sprintf("%.2f", $value);
        }
        else {
            $value = "undef";
        }
        printf(
            "    - Input Node: %s (weight %.2f) = %s\n",
            $node ? $node->id : "undef",
            $weight,
            $value
        );
    }

    # Output nodes
    my $out = $self->{"output_references"};
    for my $node (@$out) {
        printf(
            "    - Output Node: %s (weight %.2f)\n",
            $node->id,
            $node->getInputWeight($self->id)
        );
    }

    # Error
    if (my $error = $self->getError()) {
        printf("    - Error: %.2f\n", $error);
    }
}

sub getInputWeight {
    my ($self, $id) = @_;
    my $in = $self->{"input_references"};
    for (@$in) {
        my ($node, $weight) = @$_;
        if (!defined($id) && !defined($node)) {
            return $weight;
        }
        elsif ($node->id == $id) {
            return $weight;
        }
    }

    return undef;
}

package Network;

sub new {
    my ($class) = @_;
    my $ref = {
        layers => [],
        next_node_id => 0,
        training_data => [],
    };
    bless $ref, $class;
}

sub learning_rate {
    return 0.5;
}

sub addLayer {
    # Add a layer of nodes, but don't connect them yet
    my ($self, $node_count) = @_;
    my $nodes = [];

    for (1..$node_count) {
        my $node = $self->newNode();
        push(@$nodes, $node);
    }

    my $layers = $self->{"layers"};
    push (@$layers, $nodes);
}

sub newNode {
    my ($self) = @_;
    my $node = new Node($self->{"next_node_id"});
    $self->{"next_node_id"}++;
    return $node;
}

sub solidify {
    # Connect every node in one layer to every node in the next layer
    my ($self) = @_;
    my $layers = $self->{"layers"};
    for (my $i = 0; $i < (@$layers - 1); $i++) {
        my $this_layer = $layers->[$i];
        my $next_layer = $layers->[$i+1];

        for my $this_node (@$this_layer) {
            for my $next_node (@$next_layer) {
                $next_node->addInputEdge($this_node);
                $this_node->addOutputEdge($next_node);
            }
        }
    }

    # Add blank input edges to front of network
    for my $node (@{$layers->[0]}) {
        $node->addInputEdge(undef, 1);
    }
}

sub execute {
    my ($self, $inputs) = @_;
    my $layers = $self->{"layers"};

    # Add input values to first layer
    for (my $i = 0; $i < @{$layers->[0]}; $i++) {
        my $node = $layers->[0]->[$i];
        $node->setInputValue(undef, $inputs->[$i]);
    }

    # For each of the layers, execute each node and assign value to next layer
    for my $layer (@$layers) {
        for my $node (@$layer) {
            my $result = $node->execute();
            for my $on ($node->getOutputNodes()) {
                $on->setInputValue($node->id, $result);
            }
        }
    }

    # On the last layer, store the value to results
    my $last = $layers->[-1];
    my @output = ();
    for my $node (@$last) {
        push(@output, $node->execute());
    }
    return @output;
}

sub clearTrainingData {
    my ($self) = @_;
    $self->{"training_data"} = [];
}

sub addTrainingData {
    my ($self, $inputs, $outputs) = @_;
    push(@{$self->{"training_data"}}, [$inputs, $outputs]);
}

sub trainNetwork {
    my ($self, $inputs, $outputs) = @_;

    my $layers = $self->{"layers"};
    my $last = $layers->[-1];

    # Forward propagate through the network
    my @actual_outputs = $self->execute($inputs);

    # Calculate and assign errors of last layer
    for (my $i = 0; $i < length(@actual_outputs); $i++) {
        my $delta = $actual_outputs[$i] - $outputs->[$i];
        $last->[$i]->addError($delta);
    }

    # Propagate errors backwards
    for my $layer (reverse(@$layers)) {
        for my $node (@$layer) {
            my @input_tuples = $node->getInputEdges();
            for my $input_tuple (@input_tuples) {
                my ($prev_node, $weight) = @$input_tuple;
                next unless defined($prev_node);
                $prev_node->addError($weight * $node->getError());
            }
        }
    }

    # Store the deltas
    for my $layer (@$layers) {
        for my $node (@$layer) {
            my @input_tuples = $node->getInputEdges();
            for my $input_tuple (@input_tuples) {
                my ($prev_node, $weight) = @$input_tuple;
                if (defined($prev_node)) {
                    my $o_i = $prev_node->getOutputValue();
                    my $o_j = $node->getOutputValue();
                    my $err = $node->getError();
                    my $d_j = $o_j * (1-$o_j) * $err;
                    my $delta = $o_i * $d_j * $self->learning_rate() * -1;
                    $node->addWeightDelta($prev_node->id, $delta);
                }
            }
        }
    }

}

sub train {
    my ($self, $count) = @_;
    my $layers = $self->{"layers"};

    for (1..$count) {
        # Clear the deltas
        for my $layer (reverse(@$layers)) {
            for my $node (@$layer) {
                $node->clearWeightDelta();
            }
        }

        # Get all the weight adjustments
        for my $tuple (@{$self->{"training_data"}}) {
            my ($inputs, $outputs) = @$tuple;
            $self->trainNetwork($inputs, $outputs);
        }

        # Adjust the weights
        for my $layer (@$layers) {
            for my $node (@$layer) {
                my @input_tuples = $node->getInputEdges();
                $node->clearInputEdges();
                for my $input_tuple (@input_tuples) {
                    my ($prev_node, $weight) = @$input_tuple;
                    if ($prev_node) {
                        my $delta = $node->getWeightDelta($prev_node->id);
                        $weight += $delta;
                    }
                    $node->addInputEdge($prev_node, $weight);
                }
            }
        }

    }
}

sub print {
    my ($self) = @_;

    my $layers = $self->{"layers"};
    for (my $i = 0; $i < @$layers; $i++) {
        print "Layer $i\n";
        my @nodes = @{$layers->[$i]};
        for my $node (@nodes) {
            $node->print();
            print "\n";
        }
    }
}
