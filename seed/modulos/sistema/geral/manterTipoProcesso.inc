<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Tipo de Processo' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			tpcdsc,
    			tpcstatus    			
            FROM seed.tipoprocesso
            
            WHERE tpcid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['tpcdsc'].'_'.$dados[0]['tpcstatus'].'_'.$dados[0]['tpcid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['tpcid'] == '' ){
    if( $_POST['alterar'] != ''){

$tpcdsc	= $_REQUEST['tpcdsc'];

$sql = "UPDATE
			seed.tipoprocesso SET
			
				    tpcdsc	= '".$tpcdsc."'
				    
			WHERE
				tpcid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoProcesso&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$tpcdsc	= $_REQUEST['tpcdsc'];

$sql = "INSERT INTO
		seed.tipoprocesso(

	tpcdsc,
	tpcstatus
)
VALUES(
	'".$tpcdsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoProcesso&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$tpcidDel = $_REQUEST['tpcid'];

if ($tpcidDel){

$sql = "UPDATE 
   					seed.tipoprocesso
   		SET
   			tpcstatus='I'
   				WHERE
		   			tpcid =	'".$tpcidDel."'";
		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoProcesso&acao=A'</script>");
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
                <td class ="SubTituloDireita" align="right">Tipo de Processo: </td>
                <td>
                     <?= campo_texto('tpcdsc', 'S', $somenteLeitura, '', 50, 600, '', '', 'left', '',  0, 'id="tpcdsc" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirTipo();" value="GRAVAR">     
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
                        onclick=\"javascript: alterar('||tpcid ||' );\"
                        title=\"Alterar Tipo\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||tpcid ||' );\"
                        title=\"Excluir Tipo\"
                     > ' as acao, 
                tpcdsc
            FROM
                seed.tipoprocesso
            WHERE
            	tpcstatus='A'
                
            ORDER BY tpcdsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Tipo" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
    
?>
<script>


function incluirTipo(){
    var tpcid			= document.getElementById('tpcid');
    var tpcdsc			= document.getElementById('tpcdsc');

    if( tpcdsc.value == ''){
        alert( 'Campo Tipo é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( tpcid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterTipoProcesso&acao=A&tpcid='+tpcid;
    }
}
function alterar(id){
 
    var tpcid				= document.getElementById('tpcid');
    var tpcdsc				= document.getElementById('tpcdsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterTipoProcesso&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var Tipo		= arRes[0];
                                         
                                         tpcdsc.value				= Tipo;
                                         alterar.value				= id;

                                    }
                              });
}
</script>