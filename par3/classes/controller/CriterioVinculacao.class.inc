<?php
/**
 * Classe de controle do CriterioVinculacao
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_CriterioVinculacao
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */
class Par3_Controller_CriterioVinculacao extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função vincularCriteriosGuia
     * - Vincula os criterios
     *
     * @return retorna sucesso ou erro
     *
     */
    public function vincularCriteriosGuia($dados)
    {

        $parm['crtidpai'] = $dados['crtid'];
        $parm['crvvinculo'] = $dados['crtvinc'];

        $tratamento = self::deletaVinculoCriterioGuia($parm['crtidpai']);

        if( is_array($dados['criterioVinculo']) ){
            foreach( $dados['criterioVinculo'] as $crtid ){
                $criterioVinculacao = new Par3_Model_CriterioVinculacao();

                $parm['crtid'] = $crtid;
                $criterioVinculacao->popularDadosObjeto($parm);
                $criterioVinculacao->salvar();
                $criterioVinculacao->commit();

            }
        }

        echo "<script>
					alert('Operação realizada com sucesso');
                	window.location.href = 'par3.php?modulo=principal/configuracao/guia&acao=A';
				</script>";
		break;

    }//end vincularCriteriosGuia()

    /**
     * Função deletaVinculoCriterioGuia
     * - Deleta vinculo dos criterios
     *
     * @return retorna sucesso ou erro
     *
     */
    function deletaVinculoCriterioGuia($crtidpai)
    {

        $criterioVinculacao = new Par3_Model_CriterioVinculacao();

        $dadosCrtidpai = $criterioVinculacao->recuperarDadosListaVinculacao($crtidpai);
        if( is_array($dadosCrtidpai) ){
            foreach( $dadosCrtidpai as $d ){
                $criterioVinculacao->excluir($d['crvid']);
                $criterioVinculacao->commit();
            }
        }

        return true;

    }//end deletaVinculoCriterioGuia()


}//end Class
?>