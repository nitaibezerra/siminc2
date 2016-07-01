<?php

unset($_SESSION['retorno']);

// verifica se ocorre algum evento
if(isset($_REQUEST['action']) && $_POST && ($_REQUEST['action'] != '') ){
	switch($_REQUEST['action']) {
            // atualizar os dados da ação
            case "save":
//            $sql = "UPDATE painel.acao SET 
//                            acadsc='".$_POST['sbatitulo']."'
//                WHERE acaid=".$_POST['acaid'].";";
//            $db->executar($sql);

            $sql = "DELETE FROM pde.ae_acaoxmetappa WHERE acaid={$_POST['acaid']}";
            $db->executar($sql);
            
            if(is_array($_POST['ids'])) {
                foreach($_POST['ids'] as $id ) {
                    $sql = "INSERT INTO pde.ae_acaoxmetappa
                                            (acaid, mppaid) 
                            VALUES ($_POST[acaid],$id)
                       ";
                    
                    $db->pegaUm($sql);
                }
            }		

            $db->commit();

            echo "<script>
                    alert('Salvo com sucesso!');
                </script>";
            break;
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo , '');

if($_SESSION['acaid']){
    $acaid = $_SESSION['acaid'];
} else {
    $acaid = '';
}
if($acaid) {
			$sbatitulo 	   = $db->pegaLinha("SELECT acadsc FROM painel.acao WHERE acaid = '{$acaid}'");
					
			$sql = "SELECT mppa.mppaid AS codigo , mppa.mppanome AS descricao FROM pde.ae_acaoxmetappa ax
					LEFT JOIN pde.ae_metappa mppa ON mppa.mppaid = ax.mppaid
					WHERE ax.acaid = $acaid
					ORDER BY descricao";
                        
			$itens = $db->carregar($sql);
			
} 

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="action" id="action" value="save">
<input type="hidden" name="acaid"  id="acaid" value="<?=$acaid;?>">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td align='right' class="SubTituloDireita">Ação:</td>
    <td><? 
			if($acaid){
				$sql = "select acadsc from painel.acao where acaid = $acaid";
				$valor = $db->pegaum($sql);
				$sbatitulo = $valor;
				echo "<input type='hidden' name='id' value='$acaid'>";
    			echo campo_texto('sbatitulo','N','N','',75,255,'',''); 
			}
			else {
				$sbacao = $_REQUEST['sbatitulo'];
				echo "<input type='hidden' name='id' value=''>";
    			echo campo_texto('sbatitulo','N','N','',75,255,'',''); 
			}	 
		?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" valign="top">Iniciativas e Metas PPA Vinculadas:</td>
    <td >
        <table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
		<tr>
			<td style="background-color: #C9C9C9;" align="center" nowrap><input type="hidden" name="desid"></td>
			
		</tr>
		<? 
		if($itens[0]) {
			$valortotal = 0;
			$cor = 0;
			foreach($itens as $item) { 
		?>
        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="desid_<? echo $item['codigo']; ?>">
			<td align="left">
				
				<? 
				echo "<input type='checkbox' checked id='check' onclick='resultado({$item['codigo']});'>".
				$item['descricao']; ?>
				<input type='hidden' name='ids[<? echo $item['codigo']; ?>]' value='<? echo $item['codigo']; ?>'>
			</td>
		</tr>
		<? 
				$cor++;
			}
		} 
		?>
        <tr>
			<td align="right" colspan="6"><input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Vincular meta PPA"></td>
		</tr>
        </table>
    </td>
</tr>

<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter();">
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter() {
// 		if(document.formulario.sbatitulo.value==''){
// 			alert('O campo "Ação" é de preenchimento obrigatório!');
// 			return false;
// 		}
// 		else
			document.formulario.submit();

}

function abrir_lista() {
    var janela = window.open( '?modulo=sistema/tabelaapoio/cadastroacoes/listar_secretaria&acao=A&lista=meta_ppa', 'blank', 'height=600,width=500,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
    janela.focus();
}

function resultado(desid){
    var tabela = document.getElementById('orcamento');
    tabela.deleteRow(window.document.getElementById('desid_'+desid).rowIndex);
}

</script>