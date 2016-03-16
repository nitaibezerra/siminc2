<?php
header("Content-Type: text/html; charset=ISO-8859-1",true); 
// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
		
if (isset($_REQUEST[areid_filter])):
	!$_REQUEST[areid_filter] ? die() : '';	
	$sql = "
					SELECT
						DISTINCT ct.crsid,
						crstitulo
					FROM
					 cte.instrumentounidadeescola iue
					 JOIN entidade.entidade e ON e.entid = iue.entid
					 JOIN entidade.entidadedetalhe edd ON e.entid = edd.entid 
						AND (
							entdreg_medio_prof = '1' OR 
							entdreg_medio_medio = '1' OR 
							entdreg_medio_normal = '1' OR
							entdreg_medio_integrado = '1' 
						    )
					 JOIN cte.conteudoppp cpp ON cpp.entid = e.entid
					 JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid
					 JOIN cte.cursotecnico ct ON cont.crsid =  ct.crsid
					 JOIN cte.areacurso a ON a.areid = ct.areid 
					WHERE
					 a.areid = '".$_REQUEST[areid_filter]."'
					ORDER BY
						crstitulo;";
	
	$mun = $db->carregar($sql);
	
	$options .= "<option  value=\"\">Selecione um município</option>";
	foreach ($mun as $mun):
		//$options .= simec_htmlentities("<option  value=\"{$mun[muncod]}\">{$mun[mundescricao]}</option>", ENT_QUOTES);
		$options .= "<option  value=\"{$mun[crsid]}\">{$mun[crstitulo]}</option>";
	endforeach;
	
	die($options);
	
	unset($sql,$num,$options);
endif;
	
	$sqlArea = "
				SELECT
				 DISTINCT a.areid AS codigo,
				 a.aretitulo AS descricao
				FROM
				 cte.instrumentounidadeescola iue
				 JOIN entidade.entidade e ON e.entid = iue.entid
				 JOIN entidade.entidadedetalhe edd ON e.entid = edd.entid 
					AND (
						entdreg_medio_prof = '1' OR 
						entdreg_medio_medio = '1' OR 
						entdreg_medio_normal = '1' OR
						entdreg_medio_integrado = '1' 
					    )
				 JOIN cte.conteudoppp cpp ON cpp.entid = e.entid
				 JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid
				 JOIN cte.cursotecnico ct ON cont.crsid =  ct.crsid
				 JOIN cte.areacurso a ON a.areid = ct.areid; 
                ";
	$areas = $db->carregar( $sqlArea );
	
	$PstAreid = $_REQUEST[areid];
	
	$lista = array();
	if($PstAreid){
		$sqlLista="
					SELECT
						DISTINCT ct.crsid AS codigo,
						crstitulo AS descricao
					FROM
					 cte.instrumentounidadeescola iue
					 JOIN entidade.entidade e ON e.entid = iue.entid
					 JOIN entidade.entidadedetalhe edd ON e.entid = edd.entid 
						AND (
							entdreg_medio_prof = '1' OR 
							entdreg_medio_medio = '1' OR 
							entdreg_medio_normal = '1' OR
							entdreg_medio_integrado = '1' 
						    )
					 JOIN cte.conteudoppp cpp ON cpp.entid = e.entid
					 JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid
					 JOIN cte.cursotecnico ct ON cont.crsid =  ct.crsid
					 JOIN cte.areacurso a ON a.areid = ct.areid 
					WHERE
					 a.areid = '".$PstAreid."'
					ORDER BY
						crstitulo;";
		$lista = $db->carregar( $sqlLista);
		//print_r($Escolas);
	}
?>
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
		<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0" cellpadding="1" cellspacing="1">
    <tr>
      <td class="SubTituloDireita">Área:</td>
      <td>
      <select name="areid" id="areid" onChange="document.formulario.submit();">
      	<option value="">Selecione uma área</option>
      <?
      	foreach($areas as $areas){
			$ID	    = $areas['codigo'];
			$Nome   = $areas['descricao'];
			$select = $PstAreid == $ID ? 'selected' : ''; 
      ?>
        <option  value="<?=$ID?>" <?=$select?>><?=$Nome?></option>
      <?
		}
      ?>
      </select>
      </td>
      </tr>
          <? 
  if(is_array($lista) && !empty($lista)){
  
  ?>
   <tr>
	   <td class="SubTituloEsquerda" colspan="2">
	  Todos: <input name="todos" type="checkbox" id="todos"  onclick="selecionaTodos(this, '');" >
	  </td>
  </tr>
  <?
  	$maximo  = count($lista);	
//  	$Escolas = is_array($Escolas) && !empty($Escola) ? $Escolas : array(); 
  	foreach($lista as $lista){
  		$Cor = "#e2e6e7 ";
		$Divisao=$Cont%2;
		if($Divisao == 0){
			$Cor = "#fbfbfb ";
		}
		$Cont = $Cont+1;
  		?>
    <tr>
      <td style="background-color:<?=$Cor?>;">
      <input name="checkbox<?= $lista['codigo']?>" 
      type="checkbox" 
      id="checkbox<?= $lista['codigo']?>" 
      value="<?= $lista['codigo']?>"
      title="<?= $lista['descricao']?>"
      onclick="obterMarcados('checkbox<?= $lista['codigo']?>', '<?= $lista['codigo']?>','<?= $lista['descricao']?>');" />
      </td>
      <td  style="background-color:<?=$Cor?>;"><?=$lista['descricao'] ?></td>
    </tr>
      <?
  	}
  }elseif ($PstAreid){
  ?>
	<tr>
		<td colspan="2" style="color:red; text-align:center;">
			Nenhum registro foi encontrado
		</td>
	</tr>
  <?php
  }
  ?>
      <TR>
      	<td class="SubTituloDireita"></td>
      	<TD class="SubTituloEsquerda"><input name="ok" value="Ok" type="button" onclick="window.close();">&nbsp<input name="ok" value="Fechar" type="button" onclick="window.close();">&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<input name="limpar" value="Limpar filtro Cursos" type="button" onclick="selecionaTodos(document.getElementById('todos'), 1);"></TD>
      </TR>
      </table><option value=""></option>
      </form>
<script language="javascript">
objWinPai = opener.document.getElementById('cursos');
//function obterMarcados(Nome,Valor,Estado) {  
function obterMarcados() {  
	d 	      = document;
	forms     = d.formulario;
	
	objWinPai.innerHTML = '';
	var opt = '', a=0;
	
	for (i=0; i < forms.elements.length; i++) {
		if (forms.elements[i].type == 'checkbox' &&	forms.elements[i].id !== 'todos' && forms.elements[i].checked) {

			opt += "<option value=\"" + forms.elements[i].value + "\">" + forms.elements[i].title + "</option>";
			objWinPai.innerHTML = opt;
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
