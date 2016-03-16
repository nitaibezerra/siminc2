<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Cessionário' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			cesdsc,
    			cesstatus    			
            FROM seed.cessionario
            
            WHERE cesid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['cesdsc'].'_'.$dados[0]['cesstatus'].'_'.$dados[0]['cesid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['cesid'] == '' ){
    if( $_POST['alterar'] != ''){

$cesdsc	= $_REQUEST['cesdsc'];

$sql = "UPDATE
			seed.cessionario SET
			
				    cesdsc	= '".$cesdsc."'
				    
			WHERE
				cesid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterCessionario&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$cesdsc	= $_REQUEST['cesdsc'];

$sql = "INSERT INTO
		seed.cessionario(

	cesdsc,
	cesstatus
)
VALUES(
	'".$cesdsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterCessionario&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$cesidDel = $_REQUEST['cesid'];

if ($cesidDel){

		$sql = "UPDATE
			seed.cessionario SET
			
				    cesstatus	= 'I'
				    
			WHERE
				cesid =	'".$cesidDel."'";

		$db->executar($sql);
		$db->commit();	
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterCessionario&acao=A'</script>");
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
                <td class ="SubTituloDireita" align="right">Cessionário: </td>
                <td>
                     <?= campo_texto('cesdsc', 'S', $somenteLeitura, '', 50, 50, '', '', 'left', '',  0, 'id="cesdsc" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirCessionario();" value="GRAVAR">     
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
                        onclick=\"javascript: alterar('||cesid ||' );\"
                        title=\"Alterar Cessionário\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||cesid ||' );\"
                        title=\"Excluir Cessionário\"
                     > ' as acao, 
                cesdsc
            FROM
                seed.cessionario
                
            WHERE
            	cesstatus	= 'A'
                
            ORDER BY cesdsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Cessionário" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
  
?>
<script>


function incluirCessionario(){
    var cesid			= document.getElementById('cesid');
    var cesdsc			= document.getElementById('cesdsc');

    if( cesdsc.value == ''){
        alert( 'Campo Cessionário é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( cesid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterCessionario&acao=A&cesid='+cesid;
    }
}
function alterar(id){
 
    var cesid				= document.getElementById('cesid');
    var cesdsc				= document.getElementById('cesdsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterCessionario&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var cessionario		= arRes[0];
                                         
                                         cesdsc.value				= cessionario;
                                         alterar.value				= id;

                                    }
                              });
}
</script>