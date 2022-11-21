<?php
declare(strict_types=1);

$variable = "Hello, world\n";

function foo(string $var) : void
{
    write($var);
}

foo($variable);

