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
class Simec_SoapClient_ClassMap extends Simec_ArrayIterator
{
    /**
     * Adiciona um novo mapeamento ao mapeamento de classes.
     * Para incluir um tipo cuja classe de mapeamento tenha o mesmo nome,
     * basta omitir o segundo parametro.
     *
     * @param type $wsTipo O tipo do campo na definição do webservice.
     * @param type $simecClass A classe que representa o tipo em questão.
     * @return \Simec_SoapClient_ClassMap
     */
    public function add($wsTipo, $simecClass = null)
    {
        if (is_null($simecClass)) {
            $simecClass = $wsTipo;
        }
        $this->elements[$wsTipo] = $simecClass;
        return $this;
    }
}
