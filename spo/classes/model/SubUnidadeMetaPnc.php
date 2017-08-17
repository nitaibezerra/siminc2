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
class Spo_Model_SubUnidadeMetaPnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spo.subunidademetapnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'smcid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'smcid' => null,
        'suoid' => null,
        'mpnid' => null,
    );

    public function excluirPorExercicio($exercicio)
    {
        $sql = "delete from spo.subunidademetapnc where mpnid in (select mpnid from public.metapnc where prsano = '{$exercicio}')";
        return $this->executar($sql);
    }

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "select * from spo.subunidademetapnc where mpnid in (select mpnid from public.metapnc where prsano = '{$exercicio}')";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['mpnid']][] = $dado['suoid'];
        }
        return $dadosAgrupados;
    }
}