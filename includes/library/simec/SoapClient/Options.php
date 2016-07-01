<?php
/**
 *
 */

/**
 *
 */
require_once(dirname(__FILE__) . "/../ArrayIterator.php");

/**
 *
 */
class Simec_SoapClient_Options extends Simec_ArrayIterator
{
    /**
     * Adiciona uma opção do soapcliente à lista de opções utilizadas na requisição.
     *
     * @param string $key Nome da opção do soapcliente.
     * @param mixed $elem Valor para a opção.
     * @return \Simec_SoapClient_Options
     */
    public function add($key, $elem = null)
    {
        $this->elements[$key] = $elem;
        return $this;
    }
}
