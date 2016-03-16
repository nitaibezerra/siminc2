<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Interessado' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			intdsc,
    			intstatus    			
            FROM seed.interessado
            
            WHERE intid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['intdsc'].'_'.$dados[0]['intstatus'].'_'.$dados[0]['intid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['intid'] == '' ){
    if( $_POST['alterar'] != ''){

$intdsc	= $_REQUEST['intdsc'];

$sql = "UPDATE
			seed.interessado SET
			
				    intdsc	= '".$intdsc."'
				    
			WHERE
				intid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterInterresado&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$intdsc	= $_REQUEST['intdsc'];

$sql = "INSERT INTO
		seed.interessado(

	intdsc,
	intstatus
)
VALUES(
	'".$intdsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterInterresado&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$intidDel = $_REQUEST['intid'];

if ($intidDel){

$sql = "UPDATE
			seed.interessado SET
			
				    intstatus	= 'I'
				    
			WHERE
				intid =	'".$intidDel."'";

		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterInterresado&acao=A'</script>");
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
                <td class ="SubTituloDireita" align="right">Interessado: </td>
                <td>
                     <?= campo_texto('intdsc', 'S', $somenteLeitura, '', 50, 50, '', '', 'left', '',  0, 'id="intdsc" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirInteressado();" value="GRAVAR">     
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
                        onclick=\"javascript: alterar('||intid ||' );\"
                        title=\"Alterar Interessado\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||intid ||' );\"
                        title=\"Excluir Interessado\"
                     > ' as acao, 
                intdsc
            FROM
                seed.interessado
                
            WHERE
            	intstatus	= 'A'
                
            ORDER BY intdsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Interessado" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
    
?>
<script>


function incluirInteressado(){
    var intid			= document.getElementById('intid');
    var intdsc			= document.getElementById('intdsc');

    if( intdsc.value == ''){
        alert( 'Campo Interessado é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( intid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterInterresado&acao=A&intid='+intid;
    }
}
function alterar(id){
 
    var intid				= document.getElementById('intid');
    var intdsc				= document.getElementById('intdsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterInterresado&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var interessado		= arRes[0];
                                         
                                         intdsc.value				= interessado;
                                         alterar.value				= id;

                                    }
                              });
}
</script>