<?php
header("Content-Type: text/html; charset=ISO-8859-1",true); 
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$tipoensino	= $_REQUEST["tipoensino"];
	
if((isset($tipoensino)) && ($tipoensino != '')){
				$sqlListaTipoEnsino = "SELECT 
										* 
									   FROM 
									   	academico.orgao tpe
									   LEFT JOIN 
									   	academico.orgaouo teo ON teo.orgid = tpe.orgid 
									   --LEFT JOIN public.unidade uni ON uni.gunid = teo.gunid 									   
									   LEFT JOIN 
									   	entidade.funcaoentidade fe ON fe.funid = teo.funid 
									   LEFT JOIN 
									   	entidade.entidade e ON e.entid = fe.entid
									   WHERE 
									   	tpe.orgid = '". $tipoensino ."'
									   ORDER BY e.entnome 
									   ";
		$unidades = $db->carregar( $sqlListaTipoEnsino );
}
	
	
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
<? 
  if($tipoensino){
?>
    <tr>
	   <td class="SubTituloEsquerda" colspan="2">
	  Todos: <input name="todos" type="checkbox" id="todos"  onclick="selecionaTodos(this, '');" >
	  </td>
  	</tr>
 <?php
  	$maximo = count($unidades);	
  	foreach($unidades as $unidade):
  		$Cor = "#e2e6e7 ";
		$Divisao=$Cont%2;
		if($Divisao == 0){
			$Cor = "#fbfbfb ";
		}
		$Cont = $Cont+1;
  		?>
    <tr>
      <td style="background-color:<?=$Cor?>;">
      <input name="checkbox<?= $unidade['entid']?>" 
      type="checkbox"
      title="<?= $unidade['entnome']?>" 
      id="checkbox<?= $unidade['entid']?>" 
      value="<?= $unidade['entid']?>"
      onclick="obterMarcados('checkbox<?= $unidade['entid']?>', '<?= $unidade['entid']?>','<?= $unidade['entnome']?>');" />
      </td>
      <td  style="background-color:<?=$Cor?>;"><?=$unidade['entnome']?></td>

    </tr>
      <?
  	endforeach;
 }
  ?>
      <TR>
      	<td class="SubTituloDireita"></td>
      	<TD class="SubTituloEsquerda"><input name="ok" value="Ok" type="button" onclick="window.close();">&nbsp<input name="ok" value="Fechar" type="button" onclick="window.close();">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="limpar" value="Limpar filtro" type="button" onclick="selecionaTodos(document.getElementById('todos'), 1);"></TD>
      </TR>
  </table>
  <div>

  
  </div>
</form>
<script language="javascript">
objWinPai = opener.document.getElementById('unidades');

function obterMarcados() {  
	d 	      = document;
	forms     = d.formulario;
	
	objWinPai.innerHTML = '';
	var opt = '', a=0;
	
	for (i=0; i < forms.elements.length; i++) {
		if (forms.elements[i].type == 'checkbox' &&	forms.elements[i].id !== 'todos' && forms.elements[i].checked) {
			var opt = window.opener.document.createElement("OPTION") ;
			opt.value = forms.elements[i].value;
			opt.text = forms.elements[i].title;
			objWinPai.options.add(opt, objWinPai.options.length); 
			//opt += "<option value=\"" + forms.elements[i].value + "\">" + forms.elements[i].title + "</option>";
			//objWinPai.innerHTML = opt;
			a++;
		}else if (forms.elements[i].type == 'checkbox' &&	forms.elements[i].id !== 'todos') {	
			d.getElementById('todos').checked = false;
		}	
	}
	if (a == 0)
		objWinPai.innerHTML = "<option value=\"\">Duplo clique para selecionar da lista</option>";
} 

function selecionaTodos(obj, det){
	if (!obj){
		obterMarcados()		
		return;
	}	
	if (det !== '')
		obj.checked = false;
		
	var param = obj.checked ? true : false;	
	
	for (i=0; i < document.formulario.elements.length; i++){ 

		if(document.formulario.elements[i].type == "checkbox"){

      		checkBox = document.getElementById(document.formulario.elements[i].id);

		    if (checkBox.id !== 'todos') 
		    	 document.formulario.elements[i].checked=param;
		    
		}
	}
	obterMarcados()
}
</script>