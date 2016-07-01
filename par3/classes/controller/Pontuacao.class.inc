<?php
/**
 * Classe de controle da Pontuação
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Pontuacao
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */
class Par3_Controller_Pontuacao extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}

    /**
     * Função deletarCriterioGuia
     * - Deleta o critério do Guia
     *
     */
//     public function deletarCriterioGuia($crtid){
//         $oCriterioVinculacao = new Par3_Model_CriterioVinculacao();
//         $quantidade = $oCriterioVinculacao->recuperaQuantidadeVinculacao($crtid);

//         if( $quantidade == 0 ){
//             $oCriterio = new Par3_Model_Criterio();
//             $oCriterio->deletarCriterioGuia($crtid);
//             return true;
//         } else {
//             return false;
//         }
//     }


    /**
     * Função preparaSalvar
     * - Salva os critérios do instrumento
     *
     */
    public function preparaSalvar()
    {
        $inuid = $_POST['inuid'];
        $dimid = $_POST['dimid'];
        $areid = $_POST['areid'];
        $indid = $_POST['indid'];

        if( $indid != array_keys($_POST['indicador'][$dimid][$areid])[0] && array_keys($_POST['indicador'][$dimid][$areid])[0] ){
            $indid = array_keys($_POST['indicador'][$dimid][$areid])[0];
        }

//        $indid = $indid ? $indid : (array_keys($_POST['indicador'][$dimid][$areid])[0] ? array_keys($_POST['indicador'][$dimid][$areid])[0] : null );

        if(!$indid){
            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid='.$_POST['inuid'], 'error');
        }

        $ptojustificativa = $_POST['indjustificativa'][$indid];
        $ptopontuacao = 4;

        // Busca se a pontuação já foi salva
        $oPontuacao = new Par3_Model_Pontuacao;
        $ptoid = $oPontuacao->buscaPontuacao($inuid, $indid);
        $ptoid = $ptoid ? $ptoid : null;

        // Salvar Pontuacao
        $ptoid = $oPontuacao->preparaSalvar($ptoid, $inuid, $ptopontuacao, $_SESSION['usucpf'], $ptojustificativa, $indid);

        // Exclui os critérios já cadastrados
        $oPontuacaoCriterio = new Par3_Model_PontuacaoCriterio;
        $criterios = $oPontuacaoCriterio->recuperarDados($ptoid);
        if( is_array($criterios) ){
            foreach( $criterios as $criterio ){
                $oPontuacaoCriterio->preparaExcluir($criterio['ptcid']);
            }
        }

        // Salvar os critérios de resposta
        if( is_array($_POST['indicador'][$dimid][$areid][$indid]) ){
            foreach( $_POST['indicador'][$dimid][$areid][$indid] as $criterioSalvar ){
                $oPontuacaoCriterio = new Par3_Model_PontuacaoCriterio;
                $oPontuacaoCriterio->preparaSalvar($ptoid, $criterioSalvar);
            }
        }

        return $indid;

    }//end preparaSalvar()


}//end Class
?>