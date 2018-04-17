<?php

class Proposta_Controller_PrePlanoInternoLocalizacao
{
    public function salvar($dados)
    {
        switch ($dados['esfid']) {
            case Territorios_Model_Esfera::K_EXTERIOR: $campo = 'paiid'; break;
            case Territorios_Model_Esfera::K_MUNICIPAL: $campo = 'muncod'; break;
            case Territorios_Model_Esfera::K_ESTADUAL: $campo = 'estuf'; break;
        }

        $mPrePlanoInternoLocalizacao = new Proposta_Model_PrePlanoInternoLocalizacao();
        $mPrePlanoInternoLocalizacao->pliid = $dados['pliid'];
        $mPrePlanoInternoLocalizacao->excluirVarios("pliid = {$dados['pliid']}");

        if($campo && isset($dados[$campo])){

            foreach ($dados[$campo] as $valor){
                if($valor){
                    $mPrePlanoInternoLocalizacao->{$campo} = $valor;
                    $mPrePlanoInternoLocalizacao->salvar();

                    $mPrePlanoInternoLocalizacao->ploid = null;
                }
            }
        }
    } //end salvar()
}