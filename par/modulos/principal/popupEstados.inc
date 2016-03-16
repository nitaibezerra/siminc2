
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    
      <? 
      $sql = "select
                        estuf as codigo,
                        estdescricao as descricao
                    from territorios.estado
                    order by
                        estdescricao";
      $estados = $db->carregar( $sql );
  	$maximo = count($estados);	
  	foreach($estados as $estados){
  		$Cor = "#e2e6e7 ";
		$Divisao=$Cont%2;
		if($Divisao == 0){
			$Cor = "#fbfbfb ";
		}
		$Cont = $Cont+1;
  		?>
    <tr>
      <td style="background-color:<?=$Cor?>;">
      <input name="checkbox<?= $estados['codigo']?>" 
      type="checkbox"
      title="<?= $estados['descricao']?>" 
      id="checkbox<?= $estados['codigo']?>" 
      value="<?= $estados['codigo']?>"
      onclick="obterMarcados('checkbox<?= $estados['codigo']?>', '<?= $estados['codigo']?>', '<?= $estados['descricao']?>');" />
      </td>
      <td  style="background-color:<?=$Cor?>;"><?=$estados['descricao']?> </td>

    </tr>
      <?
  	}
 ?>
  <tr>
  <td>
    <input id="marcaDesmarcaTodos" type="checkbox" name="marcaDesmarcaTodos" value="marcaDesmarcaTodos" onclick="marcarDesmarcarTodos(this);"  /> Marcar / Desmarcar Todos
  </td>
  </tr>
  </table>
  <div>

  
  </div>
</form>
<script language="javascript">
var k = 0;
var t = opener.document.formulario.estados;
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

function obterMarcados(Nome,Valor,Estado) {     
	checkBox = document.getElementById(Nome); 
	if ( checkBox.checked && checkBox.id != 'marcaDesmarcaTodos' ) { 
		if((opener.document.formulario.estados.options.length == 1) && (opener.document.formulario.estados.options[0].value == "")){
			opener.document.formulario.estados.options[0] = null;
		}
	  	var d=opener.document.formulario.estados.options.length++;
		opener.document.formulario.estados.options[d].text = Estado;
		opener.document.formulario.estados.options[d].value = Valor;
		opener.document.formulario.estados.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.estados.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.estados.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.estados.options.length++;
				opener.document.formulario.estados.options[d].text = textocombogeral;
				opener.document.formulario.estados.options[d].value = "";
				//opener.document.formulario.municipios.options[d].setAttribute("","");
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