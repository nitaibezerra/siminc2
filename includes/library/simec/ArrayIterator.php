<?php
/**
 *
 */

/**
 * 
 */
class Simec_ArrayIterator implements Iterator {

    protected $elements = array();

    public function __construct($elem = null)
    {
        if (is_array($elem)) {
            $this->elements = $elem;
        }
    }

    public function current()
    {
        return current($this->elements);
    }

    public function key()
    {
        return key($this->elements);
    }

    public function valid()
    {
        $key = key($this->elements);
        return ($key !== null && $key !== false);
    }

    public function next()
    {
        return next($this->elements);
    }

    public function rewind()
    {
        reset($this->elements);
        return $this;
    }

    public function add($elem, $key = null)
    {
        if (is_null($key)) {
            array_push($this->elements, $elem);
        } else {
            $this->elements[$key] = $elem;
        }
        return $this;
    }

    public function merge(array $elems)
    {
        $this->elements = array_merge($this->elements, $elems);
    }

    public function asArray()
    {
        return $this->elements;
    }
}
