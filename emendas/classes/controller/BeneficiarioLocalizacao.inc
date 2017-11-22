<?php

class Emendas_Controller_BeneficiarioLocalizacao
{
    public function salvar($dados)
    {
        switch ($dados['esfid']) {
            case Territorios_Model_Esfera::K_EXTERIOR: $campo = 'paiid'; break;
            case Territorios_Model_Esfera::K_MUNICIPAL: $campo = 'muncod'; break;
            case Territorios_Model_Esfera::K_ESTADUAL: $campo = 'estuf'; break;
        }

        $mBeneficiarioLocalizacao = new Emendas_Model_BeneficiarioLocalizacao();
        $mBeneficiarioLocalizacao->benid = $dados['benid'];
        $mBeneficiarioLocalizacao->excluirVarios("benid = {$dados['benid']}");

        if($campo && isset($dados[$campo])){

            foreach ($dados[$campo] as $valor){
                if($valor){
                    $mBeneficiarioLocalizacao->{$campo} = $valor;
                    $mBeneficiarioLocalizacao->salvar();

                    $mBeneficiarioLocalizacao->bloid = null;
                }
            }
        }
    } //end salvar()
}