<?php
/**
 * Classe de controle da vinculação das dimensões do PAR aos indicadores do Painel
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_DimensaoPainel
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */
class Par3_Controller_DimensaoPainel extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função vincularIndicadoresPainelGuia
     * - Vincula a dimensão passada aos indicadores do painel
     *
     * @return retorna sucesso ou erro
     *
     */
    public function vincularIndicadoresPainelGuia($dimid, $indices)
    {

        $arrIndices = explode(',', $indices);


        if( is_array($arrIndices) ){
            foreach( $arrIndices as $dado ){
                $indicadorPainel = new Par3_Model_DimensaoPainel();

                $dados['dimid'] = $dimid;
                $dados['indidpainel'] = $dado;
                $indicadorPainel->popularDadosObjeto($dados);
                $indicadorPainel->salvar();
                $indicadorPainel->commit();

            }
        }

        echo "<script>
					alert('Operação realizada com sucesso');
                	window.location.href = 'par3.php?modulo=principal/configuracao/guia&acao=A';
				</script>";
		break;

    }//end vincularIndicadoresPainelGuia()


}//end Class
?>