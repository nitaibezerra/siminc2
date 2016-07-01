<?php
	// inicializa sistema
	require_once "config.inc";
	include_once "_constantes.php";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();


$sql = " SELECT 
			u.usucpf AS codigo, 
			u.usunome AS descricao, 
			u.usuemail AS usuemail,
			ent.entid AS orgcod,
			ent.entnome as orgdsc
		FROM seguranca.perfilusuario pu
		INNER JOIN seguranca.perfil p ON pu.pflcod =  p.pflcod
		INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
		INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
		INNER JOIN entidade.entidade ent ON ent.entid = u.entid
		WHERE 
			usunome IS NOT NULL
			AND us.suscod = 'A' 
			AND us.sisid = ".CONJUR_SISID." 
			AND (p.pflcod = ".PRF_EXTERNO_CONJUR.")
		GROUP BY u.usuemail, u.usunome, u.usucpf, ent.entnome, ent.entid
		ORDER BY orgdsc
";
// ver($sql);
$usuariosExternoConjur = $db->carregar( $sql );

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>


<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <table width="500px" bordercolor="#DCDCDC">
      <tr><td style="background-color:#e9e9e9"><b>Selecione:</b></td><td style="background-color:#e9e9e9"><b>Responsáveis</b></td></tr>
      <?
      if(is_array($usuariosExternoConjur)){

      		$orgao = '';
          	foreach($usuariosExternoConjur as $responsaveis):
            	$Cor = "#e2e6e7 ";
              	$Divisao=$Cont%2;
              	($Divisao == 0)? $Cor = "#fbfbfb " : "";
              	$Cont = $Cont+1;


            	if( empty($orgao) ){
					$orgao = $responsaveis['orgdsc'];
					echo "<tr><td colspan='2' style='background:#ccc;text-align:center;padding-top:5px;'><h4>".$responsaveis['orgdsc']."</h4></td></tr>";
					$Cont = 0;
				}

				if( $responsaveis['orgdsc'] != $orgao ){
					echo "</orgroup>";
					if( count($usuariosExternoConjur) != $key ){
						echo "<tr><td colspan='2' style='background:#ccc;text-align:center;padding-top:5px;'><h4>".$responsaveis['orgdsc']."</h4></td></tr>";
						$Cont = 0;
					}
				}

				?>
              	<tr>
                	<td style="background-color:<?=$Cor?>;">
                    	<input name="checkbox<?= $responsaveis['codigo']?>" type="checkbox" title="<?= $responsaveis['descricao']?>" id="checkbox<?= $responsaveis['codigo']?>" value="<?= $responsaveis['codigo']?>"
                             onclick="obterMarcados('checkbox<?= $responsaveis['codigo']?>', '<?= $responsaveis['codigo']?>','<?= str_replace("'","",$responsaveis['descricao'])?>');" />
                  	</td>
                  	<td  style="background-color:<?=$Cor?>;"><?=$responsaveis['descricao']?> </td>
              	</tr>
          	<?
          	endforeach;
          	?>
          	<tr>
            	<td>
                	<!-- <input id="marcaDesmarcaTodos" type="checkbox" name="marcaDesmarcaTodos" value="marcaDesmarcaTodos" onclick="marcarDesmarcarTodos(this);"  /> Marcar / Desmarcar Todos -->
              	</td>
              	<td>
                	<input type="button" onclick="self.close();" value="Ok" name="ok">
              	</td>
          	</tr>
      	<?
      	}else{
      		echo "Sem registros";
      	}
      	?>
    </table>
</table>

</form>
<script language="javascript">
var k = 0;
var t = opener.document.formulario.responsavel;
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

function obterMarcados(Nome,Valor,Descricao) {
	checkBox = document.getElementById(Nome);
	if ( checkBox.checked && checkBox.id != 'marcaDesmarcaTodos' ) {
		if((opener.document.formulario.responsavel.options.length == 1) && (opener.document.formulario.responsavel.options[0].value == "")){
			opener.document.formulario.responsavel.options[0] = null;
		}
	  	var d=opener.document.formulario.responsavel.options.length++;
        opener.document.formulario.responsavel.options[d].text = Descricao;
		opener.document.formulario.responsavel.options[d].name = Nome;
		opener.document.formulario.responsavel.options[d].value = Valor;
		opener.document.formulario.responsavel.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.responsavel.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.responsavel.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.responsavel.options.length++;
				opener.document.formulario.responsavel.options[d].text = textocombogeral;
				opener.document.formulario.responsavel.options[d].value = "";
				//opener.document.formulario.responsavel.options[d].setAttribute("","");
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
	 		Descricao = document.formulario.elements[i].title;
	 		obterMarcados( Nome, Valor, Descricao);
		}
	}
}

function selecionaTodos(){
	 for (i=0;i<document.formulario.elements.length;i++){ 
      if(document.formulario.elements[i].type == "checkbox"){
         document.formulario.elements[i].checked=true;
		 Nome = document.formulario.elements[i].name;
		 Valor = document.formulario.elements[i].value;
		 //Estado = document.formulario.elements[i].title;
		 obterMarcados( Nome, Valor);
		}
	}
}

</script>