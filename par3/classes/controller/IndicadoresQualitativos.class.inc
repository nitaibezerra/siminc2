<?php
/**
 * Classe de controle dos Indicadores Qualitativos
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_IndicadoresQualitativos
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-10-2015
 * @link     no link
 */
class Par3_Controller_IndicadoresQualitativos extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função desenharTela
     * - Retorna a tela que deve ser montada para o indicador.
     *
     */
    public function desenharTela(){

        switch($menu){
            case'prefeito':
                include APPRAIZ.'par3/modulos/principal/planoTrabalho/prefeito.php';
                break;
            default:
                include APPRAIZ.$pastaDadosUnidade.'prefeitura.php';
                break;
    }

}//end Class
?>