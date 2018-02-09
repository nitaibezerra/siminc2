<?php
/**
 * Classe de mapeamento da entidade monitora.ptres.
 *
 * $Id: Ptres.php 100401 2015-07-22 21:06:58Z maykelbraz $
 */

/**
 * Mapeamento da entidade monitora.ptres.
 *
 * @see Modelo
 */
class Spo_Model_SubUnidadeIndicadorPnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spo.subunidadeindicadorpnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'sicid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sicid' => null,
        'suoid' => null,
        'ipnid' => null,
    );

    public function excluirPorExercicio($exercicio)
    {
        $sql = "delete from spo.subunidadeindicadorpnc where ipnid in (select ipnid from public.indicadorpnc where prsano = '{$exercicio}')";
        return $this->executar($sql);
    }

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "select * from spo.subunidadeindicadorpnc where ipnid in (select ipnid from public.indicadorpnc where prsano = '{$exercicio}')";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['ipnid']][] = $dado['suoid'];
        }
        return $dadosAgrupados;
    }
}