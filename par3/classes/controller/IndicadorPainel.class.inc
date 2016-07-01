<?php
/**
 * Classe de controle da vinculação dos indicadores do PAR aos do Painel
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_IndicadorPainel
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */
class Par3_Controller_IndicadorPainel extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função vincularIndicadoresPainelGuia
     * - Vincula o indicador passado aos indicadores do painel
     *
     * @return retorna sucesso ou erro
     *
     */
    public function vincularIndicadoresPainelGuia($indid, $indices)
    {

        $arrIndices = explode(',', $indices);


        if( is_array($arrIndices) ){
            foreach( $arrIndices as $dado ){
                $indicadorPainel = new Par3_Model_IndicadorPainel();

                $dados['indid'] = $indid;
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