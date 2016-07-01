<?php
header("Content-Type: text/html; charset=ISO-8859-1",true); 

$entids	= $_REQUEST["institutos"];
$entids = str_replace("{},","",$entids);
$entids = str_replace("{}","",$entids);
if( $entids != '' ){
	$entids = 'AND entinst.entid in ('.$entids.')';
}

$sql = "SELECT 
			entcampus.entid as entid,
			entcampus.entnome || ' - ' || entinst.entnome as entnome
		FROM  
			entidade.entidade entcampus 
		INNER JOIN 
			entidade.funcaoentidade  fn ON entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A'
		INNER JOIN 
			entidade.funentassoc      a ON a.fueid = fn.fueid
		INNER JOIN 
			entidade.entidade   entinst ON entinst.entid = a.entid 
		INNER JOIN 
			entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid 
										   AND fn2.funid = 11 
										   AND entinst.entstatus = 'A'
										   {$entids} ";
$unidades = $db->carregar( $sql );
	
	
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
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
objWinPai = opener.document.getElementById('campus');

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