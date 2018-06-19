<?php

class Public_Controller_AreaCultural
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/area_cultural_form&acao=A';

        try {
            $mdareaCultural = new Monitora_Model_PiModalidadeEnsino($dados['mdeid']);
            $mdareaCultural->popularDadosObjeto($dados);
            $mdareaCultural->mdeano = $mdareaCultural->mdeano ? $mdareaCultural->mdeano : $_SESSION['exercicio'];
//            ver($mdareaCultural,d);

            $mdareaCultural->salvar(null,null, ['mdedsc', 'mdestatus', 'mdeano', 'mdecod']);
            $mdareaCultural->commit();
            simec_redirecionar($url . '&mdeid=' . $mdareaCultural->mdeid, 'success');
        } catch (Exception $e){
            $mdareaCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($mdeid)
    {
        $url = '?modulo=apoio/area_cultural&acao=A';

        try {
            $mdareaCultural = new Monitora_Model_PiModalidadeEnsino($mdeid);

            if($mdareaCultural->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/area_cultural_form&acao=A&capid=' . $mdareaCultural->mdeid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mdareaCultural->mdestatus = 'I';
            $mdareaCultural->salvar();
            $mdareaCultural->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdareaCultural->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
