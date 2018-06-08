<?php

class Public_Controller_ManutencaoSubItem
{
    public function salvar($dados)
    {
		$url = '?modulo=apoio/item-manutencao-form&acao=A&maiid=' . $dados['maiid'];
        try {
            $mManutencaoSubItem = new Planacomorc_Model_ManutencaoSubItem($dados['masid']);
			$dados['prsano'] = empty( $dados['prsano'] ) ? $_SESSION['exercicio'] : $dados['prsano'];
            $mManutencaoSubItem->popularDadosObjeto($dados);
            $mManutencaoSubItem->salvar(null, null, ['maiid', 'masdescricao']);
            $mManutencaoSubItem->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mManutencaoSubItem->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($masid)
    {
        $mManutencaoSubItem = new Planacomorc_Model_ManutencaoSubItem((int)$masid);
        $url = '?modulo=apoio/item-manutencao-form&acao=A&maiid=' . $mManutencaoSubItem->maiid;
        try {
            $mManutencaoSubItem->masstatus = 'I';
            $mManutencaoSubItem->salvar(null, null, ['maiid', 'masdescricao']);
            $mManutencaoSubItem->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mManutencaoSubItem->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
