<?php
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	$sqlEstado = "select estuf as codigo,
						estdescricao as descricao
					from territorios.estado
                    order by
                        estdescricao
                ";
	$estados = $db->carregar( $sqlEstado );
	
	$having = '';
	$where = '';
	$ppsid = $_REQUEST['ppsid'];	
	
	if ($_POST['moeempenhadoflag'] != 'S') {
		$where = " and  ( o.obrid NOT IN (SELECT obrid FROM obras2.mobiliarioempenhado
	                                          WHERE moeid IN (
	                                                          SELECT MAX(moeid)
	                                                          FROM obras2.mobiliarioempenhado
	                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')
	               OR o.obrid not in ( select distinct so.obrid
	               						from par.subacaoobravinculacao so
                                        	inner JOIN par.empenhosubacao eo ON eo.sbaid = so.sbaid and eo.eobano = so.sovano and eobstatus = 'A'
                                        	inner join par.subacao s ON s.sbaid = eo.sbaid  and ppsid in (924, 925, 906, 914, 913, 904)
                                            inner join par.empenho e on eo.empid = e.empid and  e.empsituacao <> 'CANCELADO' and empstatus = 'A') 
                        ) ";
	}
	
	//if( $_POST['requisicao'] == 'pesquisar' || $estados){
		//if( !empty($_POST["estados"]) ) $where .= " and m.estuf = '".$_POST["estados"]."'";
		if( is_array($_POST["esdid"]) && !empty($_POST["esdid"][0]) ){
			$where .= " and d.esdid in (".implode(', ', $_POST["esdid"]).")";
		}
		
		if ($_POST['moeempenhadoflag'] == 'S') {
            $where .= " and ( o.obrid IN (SELECT obrid FROM obras2.mobiliarioempenhado
                                          WHERE moeid IN (
                                                          SELECT MAX(moeid)
                                                          FROM obras2.mobiliarioempenhado
                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')
                       OR o.obrid in ( select distinct so.obrid 
                                        from par.subacaoobravinculacao so
                                        	inner JOIN par.empenhosubacao eo ON eo.sbaid = so.sbaid and eo.eobano = so.sovano and eobstatus = 'A'
                                        	inner join par.subacao s ON s.sbaid = eo.sbaid  and ppsid in (924, 925, 906, 914, 913, 904)
                                            inner join par.empenho e on eo.empid = e.empid and  e.empsituacao <> 'CANCELADO' and empstatus = 'A')
                            ) ";
        }
        
		if( !empty($_POST["qtdobras"]) ) $having = " having COUNT(o.obrid) = '".$_POST["qtdobras"]."'";
		
		if( in_array($ppsid, array( 924, 925 )) || in_array($ppsid, array( 906, 914 )) ){ #tipo A e B
			$where .= ' and o.tpoid in (16, 9, 104) ';
		}elseif( in_array($ppsid, array( 913, 904 )) ){ #tipo C
			$where .= ' and o.tpoid in (10, 105) ';
		}
		
		$sql="SELECT 
				m.muncod as codigo, 
				m.estuf as estados,
				m.mundescricao as nome
			FROM obras2.obras  o
				inner join entidade.endereco e on e.endid = o.endid
				inner join obras2.empreendimento ep on ep.empid = o.empid
				inner join territorios.municipio m on m.muncod = e.muncod
                inner join workflow.documento d on d.docid = o.docid
			WHERE 
				ep.empesfera = 'M'
				and o.obridpai IS NULL
				and o.obrstatus = 'A'
				and ep.prfid = 41
        		and ep.orgid = 3
                $where
			GROUP BY ep.empesfera, m.estuf, m.muncod, m.mundescricao
            	$having
			ORDER BY m.estuf, m.muncod,  m.mundescricao";
       // dbg($sql,1);
		$municipios = $db->carregar( $sql );
	//}
	$municipios = $municipios ? $municipios : array();
	/*
	 *<?
			$sqlEstado = "
                    select
                        estuf as codigo,
                        estdescricao as descricao
                    from territorios.estado
                    order by
                        estdescricao
                ";
			$estados = $_REQUEST['estados'];
			$db->monta_combo("estados", $sqlEstado, "S", "Todos", "", "", "", "200", "N", "estados");
      		?>
*/
	
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" name="requisicao" value="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <!--  <tr>
    	<td class="SubTituloDireita" width="30%">Estados:</td>
      	<td> <select name="estados" id="estados">
      			<?foreach($estados as $estados){
					$ID	  = $estados['codigo'];
					$Nome = $estados['descricao'];
					
					$select = '';
					if( $ID == $_REQUEST['estados'] ) $select = 'selected="selected"';
      			?>
        		<option  value="<?=$ID?>" <?=$select ?>><?=$Nome?></option>
      			<?}?>
      		</select>
      	</td>
    </tr>-->
    <tr>
    	<td class="SubTituloDireita" width="25%">Mobiliário Empenhado:</td>
    	<td width="75%">
    		<input type="radio" name="moeempenhadoflag" id="" value="S" <?= ( $_POST["moeempenhadoflag"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="moeempenhadoflag" id="" value="N" <?= ( ($_POST["moeempenhadoflag"] == "N" || $_POST["moeempenhadoflag"] == "") ? "checked='checked'" : "" ) ?>/> Não
            <!--  <input type="radio" name="moeempenhadoflag" id="" value=""  <?= ( $_POST["moeempenhadoflag"] == "" ? "checked='checked'" : "" ) ?> /> Todas-->
        </td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Situação da Obra:</td>
    	<td><?php
    		$esdid = array();
    		if( $_REQUEST['esdid'] && !empty($_POST["esdid"][0]) ){
    			$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='105' AND esdstatus='A' and esdid in (".implode(', ', $_REQUEST['esdid']).") ORDER BY esdordem";
    			$esdid = $db->carregar($sql);
    		}
    		$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='105' AND esdstatus='A' ORDER BY esdordem";
    		//$db->monta_combo("esdid", $sql, "S", "Todos", "", "", "", "200", "N", "esdid");
    		combo_popup( 'esdid', $sql, '', '400x500', 0, array(), '', 'S', false, false, 05, 400 );
    		?>
    	</td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Qtd de Obras:</td>
        <td>
        	<?php
        	$qtdobras = $_REQUEST['qtdobras'];
        	echo campo_texto('qtdobras', 'N', 'S', '', 11, 30, '[#]', '', 'right', '', 0, ''); ?>
        </td>
    </tr>
    <tr>
      	<td colspan="2" class="SubTituloDireita" style="text-align: center;"><input id="button" type="button" name="button" value="Filtrar" onclick="pesquisaMunicipios();" /></td>
    </tr>    
  	<tr>
  		<td colspan="2">
  		<table cellSpacing="1" cellPadding="3" border="0" align="center" style="width: 100%">
  		<tr>
	  		<td class="SubTituloDireita" colspan="6" style="text-align: center;">Lista de Municípios</td>
	  	</tr>
	  	<tr>
	  		<th>Ação</th>
	  		<th>Estado</th>
	  		<th>Municípios</th>
	  	</tr>
      <? 
      
  if(isset($estados)){
  	$maximo = count($municipios);
  	if( $maximo > 0 ){
	  	foreach($municipios as $municipios){
	  		$Cor = "#e2e6e7 ";
			$Divisao=$Cont%2;
			if($Divisao == 0){
				$Cor = "#fbfbfb ";
			}
			$Cont = $Cont+1;
	  		?>
	    <tr>
	      <td style="background-color:<?=$Cor?>; text-align: center;">
		      <input name="checkbox<?= $municipios['codigo']?>" type="checkbox" title="<?= $municipios['estados']." - ".$municipios['nome']?>" id="checkbox<?= $municipios['codigo']?>" 
		      value="<?= $municipios['codigo']?>" onclick="obterMarcados('checkbox<?= $municipios['codigo']?>', '<?= $municipios['codigo']?>','<?= str_replace("'","",$municipios['nome'])." - ".$municipios['estados']?>');" />
	      </td>
	      <td style="background-color:<?=$Cor?>;"><?=$municipios['estados']?> </td>
	      <td style="background-color:<?=$Cor?>;"><?=$municipios['nome']?> </td>
	    </tr>
	      <?
	  	}
	} else {
		$html .= '<table align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" style="width: 100%">';
		$html .= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr></table>';
		echo $html;
	}
 ?>
 </table></td></tr>
  <tr>
	  <td colspan="2">
	  		<input id="marcaDesmarcaTodos" type="checkbox" name="marcaDesmarcaTodos" value="marcaDesmarcaTodos" onclick="marcarDesmarcarTodos(this);"  /><b>Marcar / Desmarcar Todos</b>
	  </td>
  </tr>
  <?
 }
  ?>

  </table>
  <div>

  
  </div>
</form>
<script language="javascript">
var k = 0;
var t = opener.document.formulario.municipiosMobEquip;
var a = document.formulario.elements;
for(k; k< a.length; k++){
	var elementoatual = a[k];
	switch(elementoatual.type){
	case "checkbox":
		for(i=0;i<t.length;i++){
		var item = t.options[i];
			if(item.value == elementoatual.value){
				elementoatual.checked = true;
			}
		}	
	break;
	default:
		continue;
	break;
	}
}

function pesquisaMunicipios(){
	/*if( document.getElementById('estados').value == '' ){
		alert('Informe o estado');
		return false;
	}
	
	if( document.getElementById('esdid').value == '' ){
		alert('Informe a situação');
		return false;
	}*/
	
	if( $('[name="moeempenhadoflag"]:checked').val() == '' ){
		alert('Informe Mobiliário Empenhado');
		return false;
	}
	selectAllOptions( document.getElementById( 'esdid' ) );
	$('[name="requisicao"]').val('pesquisar');
	document.getElementById('formulario').submit();
}

function obterMarcados(Nome,Valor,Estado) {     
	checkBox = document.getElementById(Nome); 
	if ( checkBox.checked && checkBox.id != 'marcaDesmarcaTodos' ) { 
		if((opener.document.formulario.municipiosMobEquip.options.length == 1) && (opener.document.formulario.municipiosMobEquip.options[0].value == "")){
			opener.document.formulario.municipiosMobEquip.options[0] = null;
		}
	  	var d=opener.document.formulario.municipiosMobEquip.options.length++;
		opener.document.formulario.municipiosMobEquip.options[d].text = Estado;
		opener.document.formulario.municipiosMobEquip.options[d].value = Valor;
		opener.document.formulario.municipiosMobEquip.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.municipiosMobEquip.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.municipiosMobEquip.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.municipiosMobEquip.options.length++;
				opener.document.formulario.municipiosMobEquip.options[d].text = textocombogeral;
				opener.document.formulario.municipiosMobEquip.options[d].value = "";
				//opener.document.formulario.municipiosMobEquip.options[d].setAttribute("","");
			} 
		}
	}   
} 

function marcarDesmarcarTodos(checkbox){
	for (i=0;i<document.formulario.elements.length;i++){
		if(document.formulario.elements[i].type == "checkbox"){
      		if(checkbox.checked == true){
         		document.formulario.elements[i].checked=true;
		 	} else {
         		document.formulario.elements[i].checked=false;		 	
		 	}
	 		Nome = document.formulario.elements[i].name;
	 		Valor = document.formulario.elements[i].value;
	 		Estado = document.formulario.elements[i].title;
	 		obterMarcados( Nome, Valor,Estado);
		}
	}
}

function selecionaTodos(){
	 for (i=0;i<document.formulario.elements.length;i++){ 
      if(document.formulario.elements[i].type == "checkbox"){
         document.formulario.elements[i].checked=true;
		 Nome = document.formulario.elements[i].name;
		 Valor = document.formulario.elements[i].value;
		 Estado = document.formulario.elements[i].title;
		 obterMarcados( Nome, Valor,Estado);
		}
	}
}

</script>