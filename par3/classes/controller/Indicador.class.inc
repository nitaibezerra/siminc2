<?php
/**
 * Classe de controle do Indicador
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Indicador
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */
class Par3_Controller_Indicador extends Modelo
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
     * Função salvarIndicadorGuia
     * - Salva o indicador no Guia
     *
     */
    public function salvarIndicadorGuia()
    {
        // Salvar Indicador
        $oIndicador = new Par3_Model_Indicador;
        $indid = $oIndicador->preparaSalvar();

        // Exclui as metas já vinculadas
        $oIndicadorMeta = new Par3_Model_IndicadorMeta;
        $metasSalvas = $oIndicadorMeta->recuperarDados($indid);
        if( is_array($metasSalvas) ){
            foreach( $metasSalvas as $metas ){
                $oIndicadorMeta->preparaExcluir($metas['inmid']);
            }
        }

        // Salvar as Metas do PNE vinculadas ao Indicador
        if( is_array($_POST['metas']) ){
            foreach( $_POST['metas'] as $meta ){
                $oIndicadorMeta = new Par3_Model_IndicadorMeta;
                $oIndicadorMeta->preparaSalvar($indid, $meta);
            }
        }

        return true;

    }//end salvarIndicadorGuia()


}//end Class
?>