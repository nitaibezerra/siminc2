<?php

class Monitora_Controller_CategoriaApropriacao
{
    public function salvar()
    {
        $url = '?modulo=apoio/modalidade-pactuacao&acao=A';

        try {
            $mCategoriaApropriacao = new Monitora_Model_CategoriaApropriacao($_REQUEST['capid']);
            $mCategoriaApropriacao->popularDadosObjeto();
            $mCategoriaApropriacao->capano = $mCategoriaApropriacao->capano ? $mCategoriaApropriacao->capano : $_SESSION['exercicio'];

            $mCategoriaApropriacao->salvar(null, null, ['capcod', 'capdsc', 'capano', 'capstatus', 'capsiconv', ]);
            $mCategoriaApropriacao->commit();
            simec_redirecionar($url . '&capid=' . $mCategoriaApropriacao->capid, 'success');
        } catch (Exception $e){
            $mCategoriaApropriacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            