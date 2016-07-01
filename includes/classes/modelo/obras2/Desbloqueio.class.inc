<?php

class Desbloqueio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.desbloqueio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dbqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dbqid' => '',
        'dbqesfera' => null,
        'estuf' => null,
        'muncod' => null,
        'dbqobservacao' => null,
        'dbqjustificativa' => null,
        'dbqdtinclusao' => null,
        'dbqdtinicio' => null,
        'dbqdttermino' => null,
        'dbqdias' => null,
        'usucpf' => null,
        'dbqstatus' => null
    );

    public function listaSql($filtros)
    {
        $where = array();
        $acao = "'<center>
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: desbloqueio(\'' || d.dbqid || '\');\"
                        title=\"Alterar Desbloqueio\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluirDesbloqueio(\'' || d.dbqid || '\');\"
                        title=\"Alterar Desbloqueio\">
	 		        </center>' AS acao,";


        if ($filtros['dbqesfera'] != '') {
            $where[] = "d.dbqesfera = '" . $filtros['dbqesfera'] . "'";
        }
        if ($filtros['muncod'] != '') {
            $where[] = "d.muncod = '" . $filtros['muncod'] . "'";
        }
        if ($filtros['estuf'] != '') {
            $where[] = "d.estuf = '" . $filtros['estuf'] . "'";
        }
        if($filtros['action']=='xls')
            $acao = '';

        $sql = "
            SELECT
              {$acao}
              CASE WHEN d.dbqesfera = 'M' THEN
                'Municipal'
              WHEN d.dbqesfera = 'E' THEN
                'Estadual'
              ELSE
                'Federal'
              END esfera,
              d.estuf,
              M.mundescricao,
              d.dbqjustificativa,
              u.usunome,
              TO_CHAR(d.dbqdtinicio, 'dd/mm/yyyy') dbqdtinicio,
              TO_CHAR(d.dbqdttermino, 'dd/mm/yyyy') dbqdttermino
            FROM obras2.desbloqueio d
            LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpf
            LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
            WHERE 1 = 1 " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
        ";

        return $sql;
    }
}