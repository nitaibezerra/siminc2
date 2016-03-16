<?php
/**
 * Classe de controle do Criterio
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Criterio
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */
class Par3_Controller_Criterio extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função recuperarDadosListaVinculacao
     * - Retorn os critérios irmãos do critério passado para a vinculação das regras.
     *
     */
    public function recuperarDadosListaVinculacao($crtid){
        $oCriterio = new Par3_Model_Criterio();
        $oCriterio->carregarPorId($crtid);

        $oIndicador = new Par3_Model_Indicador();
        $oIndicador->carregarPorId($oCriterio->indid);

        return $oCriterio->recuperarDadosListaVinculacao($oIndicador->indid, $crtid);
    }

    /**
     * Função deletarCriterioGuia
     * - Deleta o critério do Guia
     *
     */
    public function deletarCriterioGuia($crtid){
        $oCriterioVinculacao = new Par3_Model_CriterioVinculacao();
        $quantidade = $oCriterioVinculacao->recuperaQuantidadeVinculacao($crtid);

        if( $quantidade == 0 ){
            $oCriterio = new Par3_Model_Criterio();
            $oCriterio->deletarCriterioGuia($crtid);
            return true;
        } else {
            return false;
        }
    }


    /**
     * Função reordenarItens
     * - Reordena os critérios de um indicador.
     *
     */
    public function reordenarItens($crtid, $direcao)
    {
        $criterio = new Par3_Model_Criterio($crtid);

        $novaordem = $direcao == 'cima' ? $criterio->crtcod - 1 : $criterio->crtcod + 1;

        $novaordem = $novaordem == 0 ? 1 : $novaordem;

        $arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($criterio->indid);

        foreach( $arCriterios as $dado ){
            $i++;

            if( !$ar[$i] ){
                if( $direcao == 'cima' && $novaordem > $dado['crtcod'] ){ // Não acontece nada
                    $ar[$i] = $dado['crtid'];
                } elseif( $direcao == 'cima' && $novaordem == $dado['crtcod']){
                    $ar[$i] = $crtid;
                    if( $dado['crtid'] != $crtid ){
                        $ar[$i + 1] = $dado['crtid'];
                    }
                } elseif( $direcao == 'cima' && $novaordem < $dado['crtcod'] ){
                    $ar[$i] = $dado['crtid'];
                }

                if( $direcao == 'baixo' && $novaordem < $dado['crtcod'] ){
                    $ar[$i] = $dado['crtid'];
                } elseif( $direcao == 'baixo' && $novaordem > $dado['crtcod']){
                    $ar[$i] = $dado['crtid'];
                } elseif( $direcao == 'baixo' && $novaordem == $dado['crtcod']){
                    $ar[$i] = $crtid;
                    $ar[$i - 1] = $dado['crtid'];
                }
            }
        }

        foreach( $ar as $ordem => $crit ){
            $oCriterio = new Par3_Model_Criterio;
            $oCriterio->atualizarOrdemCriterioGuia($crit, $ordem);
        }

        return true;

    }//end salvarEndereco()


}//end Class
?>