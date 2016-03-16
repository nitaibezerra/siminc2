<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Tipo de Documento' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			taxdsc,
    			taxstatus    			
            FROM seed.tipoanexo
            
            WHERE taxid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['taxdsc'].'_'.$dados[0]['taxstatus'].'_'.$dados[0]['taxid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['taxid'] == '' ){
    if( $_POST['alterar'] != ''){

$taxdsc	= $_REQUEST['taxdsc'];

$sql = "UPDATE
			seed.tipoanexo SET
			
				    taxdsc	= '".$taxdsc."'
				    
			WHERE
				taxid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoDocumento&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$taxdsc	= $_REQUEST['taxdsc'];

$sql = "INSERT INTO
		seed.tipoanexo(

	taxdsc,
	taxstatus
)
VALUES(
	'".$taxdsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoDocumento&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$taxidDel = $_REQUEST['taxid'];

if ($taxidDel){

$sql = "UPDATE
			seed.tipoanexo SET
			
				    taxstatus	= 'I'
				    
			WHERE
				taxid =	'".$taxidDel."'";

		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoDocumento&acao=A'</script>");
		exit();	
	}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <input type="hidden" name="submetido" value="1">
    <input type ="hidden" name="alterar" id="alterar" value="">
           
            <tr>
                <td class ="SubTituloDireita" align="right">Documento: </td>
                <td>
                     <?= campo_texto('taxdsc', 'S', $somenteLeitura, '', 50, 50, '', '', 'left', '',  0, 'id="taxdsc" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirProcesso();" value="GRAVAR">     
                </td>
            </tr>
     </table>
     </form>
          
    <?php
    $sql = "SELECT
                '<img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterar('||taxid ||' );\"
                        title=\"Alterar Documento\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||taxid ||' );\"
                        title=\"Excluir Documento\"
                     > ' as acao, 
                taxdsc
            FROM
                seed.tipoanexo
                
            WHERE
            	taxstatus	= 'A'
                
            ORDER BY taxdsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Documento" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
    
?>
<script>


function incluirProcesso(){
    var taxid			= document.getElementById('taxid');
    var taxdsc			= document.getElementById('taxdsc');

    if( taxdsc.value == ''){
        alert( 'Campo Documento é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( taxid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterTipoDocumento&acao=A&taxid='+taxid;
    }
}
function alterar(id){
 
    var taxid				= document.getElementById('taxid');
    var taxdsc				= document.getElementById('taxdsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterTipoDocumento&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var documento		= arRes[0];
                                         
                                         taxdsc.value				= documento;
                                         alterar.value				= id;

                                    }
                              });
}
</script>