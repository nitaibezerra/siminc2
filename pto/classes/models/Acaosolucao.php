<?php

include_once APPRAIZ . 'www/planacomorc/_funcoes.php';

class Model_Acaosolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'acaosolucao';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['acsid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['acaid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
    }

    public function getOptionsAcao( $where = null, $dados = array() )
    {
        if( !empty($dados) && is_null($where) && !empty($dados['temid']) && !in_array(6, $dados['temid']) ){
            $strTemid = implode(',', $dados['temid']);
            $where = " AND temid IN ( {$strTemid} ) ";
        }
        $acoes = $this->getAcoes($where);
		if(empty($acoes)){
			$acoes = array();
		}
        return $this->getOptions($acoes, array(), 'acaid');
    }

    public function getAcoes($where = null)
    {
        $sql = "SELECT acaid as codigo, acadsc as descricao FROM painel.acao WHERE acastatus = 'A' {$where} ORDER BY acadsc ";
        return $this->_db->carregar($sql);
    }

    public function getOptionsAcaoBySolucao()
    {
        $solid = $_SESSION['solid'];
        $sql = "SELECT acao.acaid as codigo, acao.acadsc as descricao , acao_solucao.*
                FROM painel.acao acao
                INNER JOIN pto.acaosolucao acao_solucao ON acao_solucao.acaid = acao.acaid
                WHERE acastatus = 'A'
                AND acao_solucao.solid = {$solid}
                ORDER BY acadsc ";

        $acoes = $this->_db->carregar($sql);
        if ($acoes === false) {
            $acoes = array();
        }
		if(empty($acoes)){
			$acoes = array();
		}
        return $this->getOptions($acoes, array('prompt' => 'Selecione ... '), 'acaid');
    }

    public function salvarAcao($arrayAcoesEstrategicas, $idSolucao)
    {
        if (is_array($arrayAcoesEstrategicas)) {

            $this->deleteAllByValues(array('solid' => $idSolucao));

            foreach ($arrayAcoesEstrategicas as $acoesEstrategicaID) {
                $this->setAttributeValue('solid', $idSolucao);
                $this->setAttributeValue('acaid', $acoesEstrategicaID);
                $id = $this->save();
                if ($id == false) {
                    throw new Exception('Erro ao inserir Ações Estratégicas.');
                }
            }
        }
    }

    public function getAcaoPainel($arrayIds)
    {
        if (is_array($arrayIds) and count($arrayIds) > 0) {
            $dados = $this->getAcoesByIdAcaoSolucao($arrayIds);

            $arrayDescricao = array();
            if ($dados) {
                foreach ($dados as $valor) {
                    $arrayDescricao[] = $valor['acadsc'];
                }
            }
            return implode('<br> ', $arrayDescricao);
        } else {
            return false;
        }
    }

    public function getAcoesByIdAcaoSolucao($arrayIds)
    {
        $ids = implode(',', $arrayIds);
        $sql = "    SELECT acao.acaid, acao.acadsc
                            FROM painel.acao acao
                            INNER JOIN pto.acaosolucao acao_solucao ON acao_solucao.acaid = acao.acaid
                            WHERE acao_solucao.acsid in ({$ids})
                            ORDER BY acadsc
                   ";
        return $this->_db->carregar($sql);
    }

    public function tabelaFinanceiro($arrayIds)
    {
        if (is_array($arrayIds) and count($arrayIds) > 0) {
            $dados = $this->getAcoesByIdAcaoSolucao($arrayIds);
            $arrayId = array();
            if ($dados) {
                foreach ($dados as $valor) {
                    $arrayId[] = $valor['acaid'];
                }
            }
//            $arrayId[] = 88;
//            $arrayId[] = 161;
            foreach ($arrayId as $acaid) {
                $this->exibirTabelaFinanceiro($acaid);
            }
        } else {
            return false;
        }
    }

    public function getSqlTabelaFinaceira($acaid, $years){
        $resultSet = array();
        foreach ($years as $year) {
            $sql = "SELECT v.vacid, vaeid, v.acaid, exercicio, v.vaetituloorcamentario, ve.vaedescricao
                    FROM planacomorc.vinculacaoacaoestrategicaexercicio ve join planacomorc.vinculacaoacaoestrategica v USING(vacid)
                    WHERE exercicio = {$year}
                    AND v.acaid = {$acaid}";
            $dados = $this->_db->carregar($sql);
            if ($dados) {
                $resultSet[] = $dados;
            }else{
                $resultSet[] = array();
            }
        }
        return $resultSet;
    }

    public function exibirTabelaFinanceiro($acaid)
    {
        $labels = array('Despesas Empenhadas', 'Valores Pagos', 'RAP não-Proc. Pagos', 'RAP Processados Pagos');
        $years = array('2011', '2012', '2013', '2014');
        $resultSet = $this->getSqlTabelaFinaceira($acaid, $years);
        $totalH = $this->calcular($resultSet, $years);
        $i = 0;
        if (!empty($totalH)):
            ?>
            <table class="table table-striped table-bordered table-condensed tbl_verde" cellspacing="0" cellpadding="0" border="0">
                <tr>
                    <td colspan="<?= count($years)+1?>" class="alinharMeio"><b><?= $this->getDescricaoAcao($acaid) ?></b></td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                    <?php foreach ($years as $year): ?>
                        <td class="alinharMeio"><b><?= $year; ?></b></td>
                    <?php endforeach; ?>
                </tr>
                <?php foreach ($totalH as $columnArray) : ?>
                    <tr>
                        <td class="alinharMeio"><b><?= $labels[$i]; ?></b></td>
                        <?php foreach ($columnArray as $column): ?>
                            <td><?= number_format($column, 2, ",", "."); ?></td>
                        <?php endforeach; ?>
                    </tr>
                    <?php $i++; ?>
                <?php endforeach; ?>
            </table>
        <?php endif;
    }

    public function calcular($resultSet, $years)
    {
        $acao = $subacao = $pis = $pos = $totalH = array();
        if (!empty($resultSet)) {

            $keys = array('empenho', 'pagamento', 'RapNPPago', 'rp_processado_pago');
            foreach ($resultSet as $k => $result) {
                $result = $result[0];
                if (!empty($result['vaeid'])) {
                    array_push($acao, resultado_soma_acoes($result['vaeid'], $years[$k]));
                    array_push($subacao, carrega_soma_subacoes($result['vaeid'], $years[$k]));
                    array_push($pis, carrega_soma_pi($result['vaeid'], $years[$k]));
                    array_push($pos, carrega_soma_ptres($result['vaeid'], $years[$k]));
                }
            }
            foreach ($keys as $k) {
                foreach ($years as $i => $year) {
                    $totalH[$k][$year] = $acao[$i]['total'][$k] + $subacao[$i]['total'][$k] + $pis[$i]['total'][$k] + $pos[$i]['total'][$k];
                }
            }
        }
        return $totalH;
    }

    public function getDescricaoAcao($acaid){
        $sql = "SELECT acadsc FROM painel.acao WHERE acastatus = 'A' AND acaid = {$acaid} ORDER BY acadsc ";
        $dados = $this->_db->carregar($sql);
        return $dados[0]['acadsc'];
    }
}