<?

// controla o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if($_POST["entcodent"])
{
	$retorno = array();
	
	$sql = "SELECT
				entid,
				entnome
			FROM
				entidade.entidade
			WHERE
				entcodent = '{$_POST["entcodent"]}'
				AND entstatus = 'A'
				AND entidade.entescolaespecializada = 't'";
	$entidade = $db->carregar($sql);
	
	if($entidade)
	{
		$retorno["entid"] 	= $entidade[0]["entid"];
		$retorno["entnome"] = $entidade[0]["entnome"];
		$retorno["erro"] 	= "f";
	}
	else
	{
		$retorno["entid"] 	= "";
		$retorno["entnome"] = "";
		$retorno["erro"] 	= "t";
	}
	
	die(simec_json_encode($retorno)); 
}

?>

<html>
	<head>
		<title>SIMEC - Escola Acessível</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
	</head>
	<body>
		<table border="0" cellspacing="3" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="100%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Escolas <?=(($_REQUEST["tipo"]=="publica") ? "Públicas" : "Privadas")?>
					</label>
				</td>
			</tr>
		</table>
		<table width="100%" align="center" border="0" cellspacing="5" cellpadding="5" class="listagem">
			<tr bgcolor="#f4f4f4">
				<td align="right">Código INEP:</td>
				<td>
					<input type="text" name="entcodent" id="entcodent" maxlength="8" onkeyup="this.value=mascaraglobal('########',this.value);" style="text-align: left; width: 20ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" class="normal" />
				</td>
			</tr>
			<tr bgcolor="#c0c0c0">
				<td colspan="2" align="center">
					<input type="button" id="bt_incluir" value="Incluir Escola" onclick="incluirEscola('<?=$_REQUEST["tipo"]?>');">
					<input type="button" id="bt_fechar" value="Fechar Janela" onclick="self.close();">
				</td>
			</tr>
		</table>
	</body>
</html>
		

<script>
<!--

function incluirEscola(tipo)
{
	if($("#entcodent").val() != "")
	{
		$.ajax({
		      url: "?modulo=eaprincipal/popInsereEscolaDiagnostico&acao=A",
		      type: "POST",
		      data: "&entcodent=" + $("#entcodent").val(),
		      dataType: "json",
		      success: function(msg) {
		         if(msg.erro == "t")
		         {
					alert("Não existe escola cadastrada para o 'Código INEP' informado.");
		         }
		         else
		         {
		        	 if(tipo == "publica")
	        			 var entids = window.opener.document.getElementsByName("entid_publica[]");
	        		 else
	        		 	 var entids = window.opener.document.getElementsByName("entid_privada[]");

					 for(var i=0; i<entids.length; i++)
					 {
						if(entids[i].value == msg.entid)
						{
							alert("Esta escola já se encontra incluída.");
							return;
						}
					 }
	        		 
		        	 insereEscola(tipo, msg.entid, msg.entnome);
		        	 alert("Escola incluída com sucesso.");
		         }
		      }
		   });
	}
	else
	{
		alert("O campo 'Código INEP' deve ser preenchido.");
		$("#entcodent").focus();
	}
}

// função para inserção na tabela de escolas públicas/privadas
function insereEscola(tipo, entid, entnome)
{
	// insere na tabela de escolas publicas
	if(tipo == "publica") {
		var tabela = window.opener.document.getElementById("tabela_escolas_publicas");
	}
	// insere na tabela de escolas privadas
	else {
		var tabela = window.opener.document.getElementById("tabela_escolas_privadas");
	}

	// insere na tabela	
	var tamanho = tabela.rows.length;
	
	if(tamanho == 1) {			
		var linha = tabela.insertRow(tamanho);
		linha.style.backgroundColor = "#f4f4f4";
	} 
	else {
		var linha = tabela.insertRow(tamanho);
		
		if(tabela.rows[tamanho-1].style.backgroundColor == "rgb(224, 224, 224)") {
			linha.style.backgroundColor = "#f4f4f4";					
		} else {
			linha.style.backgroundColor = "#e0e0e0";					
		}
	}

	// seta o id da linha 
	linha.id = "escola_" + entid;
	
	var colAcao 	 	= linha.insertCell(0);
	var colEscola 	 	= linha.insertCell(1);

	colAcao.style.textAlign 	 	= "center";
	colEscola.style.textAlign 	 	= "center";

	if(tipo == "publica") {
		var nameText 	= "eaequantidadealuno_publica[]";
		var nameHidden	= "entid_publica[]";
	} else {
		var nameText 	= "eaequantidadealuno_privada[]";
		var nameHidden	= "entid_privada[]";
	}
	
	colAcao.innerHTML 		= '<img style="cursor:pointer;" title="Excluir escola" src="../imagens/excluir.gif" onclick="excluiEscola(\''+tipo+'\', this.parentNode.parentNode.rowIndex);" />' +
							  '<input type="hidden" name="'+nameHidden+'" value="'+entid+'" />';
	colEscola.innerHTML 	= entnome;
}

-->
</script>