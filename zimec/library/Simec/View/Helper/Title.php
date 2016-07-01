<?php
/**
 * Abstract class for extension
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 10000
 * @link     no link
 */
require_once 'Zend/View/Helper/FormElement.php';

/**
 * Simec_View_Helper_Title
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0b
 * @link     no link
 */
class Simec_View_Helper_Title extends Simec_View_Helper_Element
{


    /**
     * Função title
     * - monta title
     *
     * @param string $title    titulo.
     * @param string $subTitle subtitulo em texto (opcional).
     * @param array  $attribs  não sei pra que serve.
     *
     * @return retorna o titulo.
     * */
    public function title($title, $subTitle=null, $attribs=array())
    {
        $xhtml = '<h2>'.$title.'</h2>';

        return $this->buildField($xhtml, $subTitle, $attribs);

    }//end title()


    /**
     * Função pegarMenu
     * - função que pega os dados do menu
     *
     * @param integer $mnuid id do menu a ser recuperado.
     *
     * @return array(
     *                  'mnuid'=>123,
     *                  'mnuidpai'=>1234,
     *                  'mnudsc'=>menu,
     *                  'mnulink'=>linkmenu
     *              )
     * */
    public function pegarMenu($mnuid)
    {
        global $db;
        $sql = "SELECT mnuid, mnuidpai, mnudsc, mnulink
                FROM seguranca.menu
                WHERE mnuid = $mnuid";

        return $db->pegaLinha($sql);

    }//end pegarMenu()


    /**
     * Função completaRastro
     * - função recursiva que completa o rastro do menu cadastrado
     *
     * @param array $rastro variavel que vem com o parâmetro inicial a ser completado
     *
     * @return array(
     *                  array(
     *                      'mnuid'=>123,
     *                      'mnuidpai'=>1234,
     *                      'mnudsc'=>menu,
     *                      'mnulink'=>linkmenu
     *                  ), ...
     *              )
     * */
    public function completaRastro($rastro)
    {
        $qtd = count($rastro) - 1;
        if (($rastro[$qtd]['mnuidpai'] > 0)) {
            $rastro[] = $this->pegarMenu($rastro[$qtd]['mnuidpai']);
        }

        $qtd = count($rastro) - 1;
        if ($rastro[$qtd]['mnuidpai'] > 0) {
            $rastro[] = $this->completaRastro($rastro);
        }

        return $rastro;

    }//end completaRastro()


    /**
     * Função buildField
     * - função que monta o titulo
     *
     * @param string $xhtml   titulo.
     * @param string $label   subtitulo em texto (opcional).
     * @param array  $attribs não sei pra que serve.
     *
     * @return titulo
     * */
    public function buildField($xhtml, $label=null, $attribs=array())
    {
        // Se não possuim subtitulo.
        if ($label === '') {
            $url  = explode('=', $_SESSION['favurl']);
            $home = $url[0].'='.$_SESSION['paginainicial'];
            // Pega o mnuid da página.
            $mnuid = $_SESSION['acl'][$_SESSION['sisid']][$_SESSION['favurl']]['mnuid'];
            // Carrega dados da página.
            $menu = $this->pegarMenu($mnuid);
            // Completa rastro dando um array com a primeira posição preenchida.
            $rastro = array($menu);
            $rastro = $this->completaRastro($rastro);
            // Monta o rastro. Escreve de forma invertida pois o array vem invertido.
            $html = '';
        	if (is_array($rastro) === true) {
                foreach ($rastro as $menu) {
                    $link = '';
                    if ($menu['mnulink'] === true) {
                        $link = 'href="'.$menu['mnulink'].'"';
                    }

                    $html = '<li><a '.$link.'>'.$menu['mnudsc'].'</a>'.$html;
                }
        	}

        	$subTitle = '<li>
		    				<a href="'.$home.'">Página Inicial</a>
		                 </li>'.$html;
        } else {
        	if (!empty($label)) {
        		$subTitle = '<li><a>'.$label.'</a></li>';
        	}
        }//end if

        $html = '<div class="row wrapper border-bottom white-bg page-heading">
	    			<div class="col-lg-12">
	    				'.$xhtml;
	    					
	    if ($subTitle) {
	    	$html.= '<ol class="breadcrumb">
		    		    '.$subTitle.'
	    			</ol>';
	    }
	    
	    $html.= '	</div>
	    		</div>';

        return $html;

    }//end buildField()


}//end class

?>