<?php
header("Content-Type: text/html; charset=ISO-8859-1",true); 
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	$sqlEstado = "SELECT DISTINCT estado.estuf as codigo,
					 estado.estdescricao AS descricao 
   		FROM cte.instrumentounidadeescola iue
        INNER JOIN entidade.entidade e ON e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
        INNER JOIN entidade.endereco ende ON ende.entid = e.entid
        INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod
		INNER JOIN territorios.estado ON estado.estuf = mu.estuf 
		INNER JOIN territorios.regiao ON regiao.regcod = estado.regcod
		INNER JOIN territorios.pais ON pais.paiid = regiao.paiid
		INNER JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
        LEFT JOIN entidade.entidadedetalhe edd ON
           e.entid = edd.entid
           AND
           (
           entdreg_infantil_preescola = '1' OR
           entdreg_fund_8_anos        = '1' OR
           entdreg_fund_9_anos        = '1' OR
           entdreg_medio_medio        = '1' OR
           entdreg_medio_medio        = '1' Or
           entdreg_medio_integrado    = '1' OR
           entdreg_medio_normal       = '1' OR
           entdreg_medio_prof         = '1'
           )
		LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid 
		LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid AND cpp.cppid IS NOT NULL 
		LEFT JOIN cte.cursotecnico ct ON cont.crsid = ct.crsid 
   		WHERE
		e.tpcid = 1 AND fe.funid = 3 
   		ORDER BY
       		descricao";
#	$estados = $db->carregar( $sqlEstado );

	$estado	= $_REQUEST["estado"];
	
	if(isset($estado)){
				$sqlListaMunicipios="
				SELECT  DISTINCT mu.muncod as codigo, 
							 mu.estuf as estados, 
							 mundescricao as nome 
   		FROM cte.instrumentounidadeescola iue
        INNER JOIN entidade.entidade e ON e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
        INNER JOIN entidade.endereco ende ON ende.entid = e.entid
        INNER JOIN territorios.municipio mu ON mu.muncod = ende.muncod
		INNER JOIN territorios.estado ON estado.estuf = mu.estuf 
		INNER JOIN territorios.regiao ON regiao.regcod = estado.regcod
		INNER JOIN territorios.pais ON pais.paiid = regiao.paiid
		INNER JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
        LEFT JOIN entidade.entidadedetalhe edd ON
           e.entid = edd.entid
           AND
           (
           entdreg_infantil_preescola = '1' OR
           entdreg_fund_8_anos        = '1' OR
           entdreg_fund_9_anos        = '1' OR
           entdreg_medio_medio        = '1' OR
           entdreg_medio_medio        = '1' Or
           entdreg_medio_integrado    = '1' OR
           entdreg_medio_normal       = '1' OR
           entdreg_medio_prof         = '1'
           )
		LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid 
		LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid AND cpp.cppid IS NOT NULL 
		LEFT JOIN cte.cursotecnico ct ON cont.crsid = ct.crsid 
   		WHERE
   		mu.estuf = '".$estado."' AND e.tpcid = 1 AND fe.funid = 3 
   		ORDER BY
       		mundescricao";
		$municipios = $db->carregar( $sqlListaMunicipios );
	}
	
	
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <tr>
      <td class="SubTituloDireita">Estados:</td>
      <td>
      <?PHP
		$db->monta_combo('estado',$sqlEstado,'S','Escolha um Estado','document.formulario.submit();','','','','','estado');		
      ?>
      </td>
    </tr>
      <? 
  if($estado){
 ?>
    <tr>
	   <td class="SubTituloEsquerda" colspan="2">
	  Todos: <input name="todos" type="checkbox" id="todos"  onclick="selecionaTodos(this, '');" >
	  </td>
  	</tr>
 <?php
  	$maximo = count($municipios);	
  	foreach($municipios as $municipios):
  		$Cor = "#e2e6e7 ";
		$Divisao=$Cont%2;
		if($Divisao == 0){
			$Cor = "#fbfbfb ";
		}
		$Cont = $Cont+1;
  		?>
    <tr>
      <td style="background-color:<?=$Cor?>;">
      <input name="checkbox<?= $municipios['codigo']?>" 
      type="checkbox"
      title="<?= $municipios['nome']." - ".$municipios['estados']?>" 
      id="checkbox<?= $municipios['codigo']?>" 
      value="<?= $municipios['codigo']?>"
      onclick="obterMarcados('checkbox<?= $municipios['codigo']?>', '<?= $municipios['codigo']?>','<?= $municipios['nome']." - ".$municipios['estados']?>');" />
      </td>
      <td  style="background-color:<?=$Cor?>;"><?=$municipios['nome']?> - <?=$municipios['estados']?> </td>

    </tr>
      <?
  	endforeach;
 }
  ?>
      <TR>
      	<td class="SubTituloDireita"></td>
      	<TD class="SubTituloEsquerda"><input name="ok" value="Ok" type="button" onclick="window.close();">&nbsp<input name="ok" value="Fechar" type="button" onclick="window.close();">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="limpar" value="Limpar filtro escolas" type="button" onclick="selecionaTodos(document.getElementById('todos'), 1);"></TD>
      </TR>
  </table>
  <div>

  
  </div>
</form>
<script language="javascript">
objWinPai = opener.document.getElementById('municipios');

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