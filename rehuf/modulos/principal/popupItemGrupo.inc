<?php
global $db;

if ( $_REQUEST['pesquisa'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	//pesquisaItemGrupoPopUp( $_REQUEST['gruid'], $_REQUEST['itecatmat'], $_REQUEST['preid'] );
	$itecatmat = $_REQUEST['itecatmat'];
	$gruid = $_REQUEST['gruid'];
	$preid = $_REQUEST['preid'];
	
	$sqlItem = "SELECT
			   (CASE WHEN (igp.preid is null) OR (igp.igpstatus = 'I') THEN
			  		('<input type=\"checkbox\" checked=\"checked\" value=\"'||ig.itgid||'_'||(case when igp.igpid is null then 'null' else ''||igp.igpid||'' end )||'_'||(case when igp.igpstatus is null then 'null' else ''||igp.igpstatus||'' end )||'\" name=\"itgid_'||ig.itgid||'\" id=\"itgid_'||ig.itgid||'\">') || ig.itgid 
			  	 ELSE ('<input type=\"checkbox\" checked=\"checked\" disabled=\"disabled\" name=\"itgid_'||ig.itgid||'\" id=\"itgid_'||ig.itgid||'\">') 
			    END) as seleciona,
			  g.grunome,
			  i.itecatmat,
			  '<b>'|| i.itedescricao ||
			  '</b><br>'||i.iteapresentacao as dados
			FROM 
			  rehuf.item i 
			  inner join rehuf.itemgrupo ig 
			  	on (i.iteid = ig.iteid) inner join rehuf.grupoitens g
			  	on (ig.gruid = g.gruid and g.grustatus = 'A')
			  left join rehuf.itemgrupopregao igp
    			on (ig.itgid = igp.itgid AND igp.preid = $preid)
			WHERE i.itestatus = 'A'
			AND ig.itgstatus = 'A'";
	if($itecatmat){
		$sqlItem.= " AND i.itecatmat = '$itecatmat'";
	}
	if($gruid){
		$sqlItem.= " AND g.gruid = '$gruid'";
	}
	
	$sqlItem.= " ORDER BY g.gruid, i.itecatmat, i.itedescricao, i.iteapresentacao";
	//exit;
	
	ver($sqlItem);
}

if ( $_REQUEST['inserir'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	inserirItemGrupoPopUp( $_REQUEST['itgid'], $_REQUEST['preid'] );
	exit;
}

monta_titulo( 'Filtro de Pesquisa', '' );

?>
<style>
@CHARSET "ISO-8859-1";

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 55%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>

<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form name="formulario" id="formulario" method="post">

<input type="hidden" name="preid" id="preid" value="<?=$_REQUEST['preid']; ?>"> 

  <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"  border="0" align="center" width="95%">
  	<tr>
  		<td class="SubTituloDireita">Grupo:</td>
		<?php
			$sqlGrupo = "SELECT 
					  gruid as codigo,
					  grunome as descricao
					FROM
					  rehuf.grupoitens
					WHERE
					  grustatus = 'A'
					order by grunome";
		?>
		<td><?= $db->monta_combo("gruid",$sqlGrupo, 'S','-- Selecione um grupo --','', '', '',250,'N','gruid');?></td>
  	</tr>
  	<tr>
  		<td class="SubTituloDireita">CATMAT:</td>
  		<td><?=campo_texto( 'itecatmat', 'N', 'S', '', 20, 10, '[#]', '','','','','id="itecatmat"'); ?></td>
  	</tr>
  	<tr>
  		<th colspan="2"><input type="button" value="Pesquisar" name="btnPesquisa" id="btnPesquisa" onclick="pesquisar();"></th>
  	</tr>
  </table>
 </form>
    <div id="lista">
  	<? //pesquisaItemGrupoPopUp('','', $_REQUEST['preid']);
 
  	if($sqlItem){
  		monta_titulo( '', 'Lista de Itens' );
  		
		$linha2 = "<label for=\"seleciona\"> <input type=\"checkbox\" checked=\"checked\" name=\"seleciona'\" id=\"seleciona\" onclick=\"selecionaTodos();\"><b>Selecionar todos</b></label>";
		
		
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
		print '<td bgcolor="#e9e9e9" align="left" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
		
		//ver($sql);
		$cabecalho = array("Selecionar", "Grupo", "CATMAT", "Descrição/Apresentação");
		
		$db->monta_lista( $sqlItem, $cabecalho, 20, 5, 'N','Center','','formulariomontalista'); ?>
		  <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"  border="0" align="center" width="95%">
		  	<tr>
		  		<th><input type="button" value="Incluir" name="btnIncluir" id="btnIncluir" onclick="incluir();"></th>
		  	</tr>
		  </table><?php
	}else{
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
		print '<td bgcolor="#e9e9e9" align="left" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'."<center style=\"color: red;\">Selecione um filtro para pesquisar itens!</center>".'</td></tr></table>';
	} 
  	?>
  </div>

  <div id="erro"></div>
 </body>
 <script type="text/javascript">
 function selecionaTodos(){
 	//checked = true;
 	
 	for(i=0; i<formulariomontalista.length; i++){
 	
 		if(formulariomontalista.elements[i].type == 'checkbox'){
 			
 			if(!formulariomontalista.elements[i].disabled){ 				
 				if($('seleciona').checked){
 					formulariomontalista.elements[i].checked = true;
 				}else{
 					formulariomontalista.elements[i].checked = false;
 				}
 			}
 		}
 	}
 }
 function incluir(){
 	var itgid = "";
 	for(i=0; i<formulariomontalista.length; i++){
 		if(formulariomontalista.elements[i].type == 'checkbox'){
 			if(!formulariomontalista.elements[i].disabled){
 				if( formulariomontalista.elements[i].id != 'seleciona' ){
 					if(formulariomontalista.elements[i].checked){
	 					if(itgid == ""){
	 						itgid = formulariomontalista.elements[i].value;
	 					}else{
	 						itgid = itgid + ',' + formulariomontalista.elements[i].value;
	 					}
	 				}
 				}
 			}
 		}
 	}
	$('loader-container').show();
	var myAjax = new Ajax.Request('rehuf.php?modulo=principal/popupItemGrupo&acao=A',{
							method:		'post',
							parameters: '&inserir=true&itgid='+itgid+
													  '&preid='+$('preid').value,
							onComplete: function(res){
								if(res.responseText == "1"){
									alert("Operação realizada com sucesso!");
									window.opener.pesquisaItemGrupoPregao();
									self.close();
								}
							}						
						});
	$('loader-container').hide();
 }
 function pesquisar(){
 
 	//$('loader-container').show();
 		window.location.href = 'rehuf.php?modulo=principal/popupItemGrupo&acao=A&preid='+$('preid').value+'&pesquisa=true&itecatmat='+$('itecatmat').value+
															'&gruid='+$('gruid').value+
															'&preid='+$('preid').value;
		/*var myAjax = new Ajax.Request('rehuf.php?modulo=principal/popupItemGrupo&acao=A',{
								method:		'post',
								parameters: '&pesquisa=true&itecatmat='+$('itecatmat').value+
															'&gruid='+$('gruid').value+
															'&preid='+$('preid').value,
								onComplete: function(res){
									$('lista').innerHTML = res.responseText;
								}						
							});*/
		//$('loader-container').hide();
 }
 </script>