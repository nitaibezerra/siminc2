<?php

class Public_Controller_SegmentoCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';

        try {
            $mdsegCultural = new Monitora_Model_PiNivelEtapaEnsino($dados['neeid']);
            $mdsegCultural->popularDadosObjeto($dados);
            $mdsegCultural->neeano = $mdsegCultural->neeano ? $mdsegCultural->neeano : $_SESSION['exercicio'];
//            ver($mdareaCultural,d);

            $mdsegCultural->salvar(null,null);
            $mdsegCultural->commit();
            simec_redirecionar($url . '&neeid= ' . $mdsegCultural->neeid.'&mdeid='.$mdsegCultural->mdeid, 'success');
        } catch (Exception $e){
            $mdsegCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($neeid)
    {
        $url = '?modulo=apoio/area_cultural&acao=A';

        try {
            $mdsegCultural = new Monitora_Model_PiNivelEtapaEnsino($neeid);
/*
            if($mdsegCultural->existeVinculo()){
                $mdsegCultural = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/area_cultural_form&acao=A&capid=' . $mdsegCultural->mdeid;
                simec_redirecionar($url, 'error', $mensagem);
            }*/

            $mdsegCultural->neestatus = 'I';
            $mdsegCultural->salvar();
            $mdsegCultural->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdsegCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function montarListagem($dados, $meeid)
    {
        $mdsegCultural = new Monitora_Model_PiNivelEtapaEnsino($dados['neeid']);
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

//        $lista = (new Monitora_Model_PiNivelEtapaEnsino()->RecuperaSegmentoPorArea($mdsegCultural->mdeid));
        include_once APPRAIZ . 'planacomorc/modulos/apoio/segmento-cultural.inc';

    }
}
