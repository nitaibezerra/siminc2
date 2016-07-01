<?php
/**
 * $Id: Operacoes.php 83465 2014-07-24 19:35:29Z maykelbraz $
 */

class Simec_Operacoes
{
    /**
     * Verifica se dois itens são iguais.
     * @param mixed $item1
     * @param mixed $item2
     * @return bool
     * @see Simec_Listagem::addRegraDeLinha
     * @see Simec_Listagem::renderDados
     */
    public static function igual($item1, $item2)
    {
        return ($item1 == $item2);
    }

    /**
     * Verifica se dois itens são diferentes.
     * @param mixed $item1
     * @param mixed $item2
     * @return bool
     * @see Simec_Listagem::addRegraDeLinha
     * @see Simec_Listagem::renderDados
     */
    public static function diferente($item1, $item2)
    {
        return ($item1 != $item2);
    }

    /**
     * Verifica se o texto do $item2 está dentro do texto do $item1.
     * @param string $item1 Texto completo (subject/haystack)
     * @param string $item2 Texto para busca (needle) dentro do $item1.
     * @return bool
     */
    public static function contem($item1, $item2)
    {
        return (!(false === strpos(strtolower($item1), strtolower($item2))));
    }
}
