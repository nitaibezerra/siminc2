<?php

class Public_Controller_Indicadorpnc
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/indicadorpnc&acao=A';

        try {
            $mIndicadorpnc = new Public_Model_IndicadorPnc($dados['ipnid']);
            $mIndicadorpnc->popularDadosObjeto($dados);
            $mIndicadorpnc->prsano = $mIndicadorpnc->prsano ? $mIndicadorpnc->prsano : $_SESSION['exercicio'];
                
            $mIndicadorpnc->salvar(null, null, ['ipndsc', 'ipnstatus', 'prsano', 'ipncod', ]);
            $mIndicadorpnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mIndicadorpnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($ipnid)
    {
        $url = '?modulo=apoio/indicadorpnc&acao=A';

        try {
            $mIndicadorpnc = new Public_Model_IndicadorPnc($ipnid);
            
            if($mIndicadorpnc->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mIndicadorpnc->ipnid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mIndicadorpnc->ipnstatus = 'I';
            $mIndicadorpnc->salvar();
            $mIndicadorpnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mIndicadorpnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            