<?php

class Public_Controller_UnidadeOrcamentaria
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria&acao=A';

        try {
            $mUnidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($dados['unoid']);
            $mUnidadeOrcamentaria->popularDadosObjeto($dados);
            $mUnidadeOrcamentaria->prsano = $mUnidadeOrcamentaria->prsano ? $mUnidadeOrcamentaria->prsano : $_SESSION['exercicio'];

            $mUnidadeOrcamentaria->salvar(null, null, ['unocod', 'prsano', 'unonome', 'unostatus','unocodigopi','unofundo','unosigla', 'unocodorcamento', ]);
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($unoid)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria&acao=A';

        try {
            $mUnidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($unoid);

            if($mUnidadeOrcamentaria->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&unoid=' . $mUnidadeOrcamentaria->unoid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mUnidadeOrcamentaria->unostatus = 'I';
            $mUnidadeOrcamentaria->salvar();
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
