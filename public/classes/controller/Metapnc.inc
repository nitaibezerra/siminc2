<?php

class Public_Controller_Metapnc
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/metapnc_form&acao=A&mpnid=';

        try {
            $mMetapnc = new Public_Model_MetaPnc($dados['mpnid']);
            $mMetapnc->popularDadosObjeto($dados);
            $mMetapnc->prsano = $mMetapnc->prsano ? $mMetapnc->prsano : $_SESSION['exercicio'];
                
            $mMetapnc->salvar(null, null, ['mpndsc', 'mpnstatus', 'mpncod','mpnnome' , 'prsano', ]);
            $mMetapnc->commit();
            simec_redirecionar($url . $mMetapnc->mpnid, 'success');
        } catch (Exception $e){
            $mMetapnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($mpnid)
    {
        $url = '?modulo=apoio/metapnc&acao=A';

        try {
            $mMetapnc = new Public_Model_MetaPnc($mpnid);
            
            if($mMetapnc->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mMetapnc->mpnid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mMetapnc->mpnstatus = 'I';
            $mMetapnc->salvar();
            $mMetapnc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMetapnc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()


    public function montarFormulario($mpnid = null, $ipnid = null)
    {
        global $simec;

        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);

        $listaIndicadores = (new Public_Model_IndicadorPnc())->recuperarTodos('*', ['mpnid = ' . (int)$mpnid]);

        if(!$podeEditar && $listaIndicadores){
            $ipnid = $ipnid? $ipnid: $listaIndicadores[0]['ipnid'];
        }

        $mIndicadorpnc = new Public_Model_IndicadorPnc($ipnid);
        // ver($mSubUnidade, d);

        if(!$mIndicadorpnc->mpnid){ $mIndicadorpnc->mpnid = $mpnid; }
        include_once APPRAIZ. 'planacomorc/modulos/apoio/indicadorpnc_form.inc';
    }

    public function montarListagem($mpnid)
    {
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

        $lista = (new Public_Model_IndicadorPnc())->recuperarTodos('*', ['mpnid = ' . (int)$mpnid, "ipnstatus = 'A'"]);
        include_once APPRAIZ . 'planacomorc/modulos/apoio/indicadorpnc.inc';
    }

}            