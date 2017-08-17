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
class Spo_Model_SubUnidadeMetaPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spo.subunidademetappa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'smpid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'smpid' => null,
        'suoid' => null,
        'mppid' => null,
    );

    public function excluirPorExercicio($exercicio)
    {
        $sql = "delete from spo.subunidademetappa where mppid in (select mppid from public.metappa where prsano = '{$exercicio}')";
        return $this->executar($sql);
    }

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "select * from spo.subunidademetappa where mppid in (select mppid from public.metappa where prsano = '{$exercicio}')";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['mppid']][] = $dado['suoid'];
        }
        return $dadosAgrupados;
    }
}