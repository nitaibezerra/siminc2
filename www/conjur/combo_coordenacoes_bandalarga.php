<?php
    // inicializa sistema
    require_once "config.inc";
    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    $db = new cls_banco();

    /*
    #BUSCA COORDENAÇÕES - NÃO É MAIS USADO.
    $sqlListaCoordenacoes = "
        SELECT  coo.coonid AS codigo,
                coo.coodsc AS descricao
        FROM conjur.coordenacao coo
        {$where}
        ORDER BY descricao
    ";
    */
    
    #OS ESTADOS DO WORKFLOW:
    # - ENCAMINHAR PARA A CGA;
    # - ENCAMINHAR PARA A CGAA;
    # - ENCAMINHAR PARA A CGAE;
    # - ENCAMINHAR PARA ANÁLISE DO GABINETE;
    # 
    # SÃO CONSIDERADOS CCOORDENAÇÕES NA VISÃO DO USUÁRIO. A SQL TRAS AS AÇÕES QUE REPRESENTA AS COORDENAÇÕES.
    $sqlListaCoordenacoes = "
        SELECT  aedid AS codigo,
		--esddsc||' - '||aeddscrealizar AS descricao 
		aeddscrealizar AS descricao 
        FROM workflow.estadodocumento doc
        JOIN workflow.acaoestadodoc aed ON aed.esdidorigem = doc.esdid
        
        WHERE aedid IN (1048,1053,1052,1054)
        
        ORDER BY descricao
    ";
      
    // ver($sqlListaCoordenacoes,d);
    $listaCoordenacoes = $db->carregar( $sqlListaCoordenacoes );
    //ver($sqlListaCoordenacoes,d);
    
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <table width="500px" bordercolor="#DCDCDC">
      <tr><td style="background-color:#e9e9e9"><b>Selecione:</b></td><td style="background-color:#e9e9e9"><b>Coordenações</b></td></tr>
      <?
      if(is_array($listaCoordenacoes)){
          foreach($listaCoordenacoes as $coordenacao):
              $Cor = "#e2e6e7 ";
              $Divisao=$Cont%2;
              ($Divisao == 0)? $Cor = "#fbfbfb " : "";
              $Cont = $Cont+1;

              ?>
              <tr>
                  <td style="background-color:<?=$Cor?>;">
                      <input name="checkbox<?= $coordenacao['codigo']?>" type="checkbox" title="<?= $coordenacao['descricao']?>" id="checkbox<?= $coordenacao['codigo']?>" value="<?= $coordenacao['codigo']?>"
                             onclick="obterMarcados('checkbox<?= $coordenacao['codigo']?>', '<?= $coordenacao['codigo']?>','<?= str_replace("'","",$coordenacao['descricao'])?>');" />
                  </td>
                  <td  style="background-color:<?=$Cor?>;"><?=$coordenacao['descricao']?> </td>

              </tr>
          <?
          endforeach;
          ?>
          <tr>
              <td style="background-color:#e9e9e9">
                  <b> Total (
                  <script>
                      var x=document.forms.formulario.status
                      window.document.write(x.options[x.selectedIndex].text)
                  </script>):
                  </b>
              </td>
              <td style="background-color:#e9e9e9"><b><?=$Cont?></b></td>
          </tr>
          <tr>
              <td>
                  <input id="marcaDesmarcaTodos" type="checkbox" name="marcaDesmarcaTodos" value="marcaDesmarcaTodos" onclick="marcarDesmarcarTodos(this);"  /> Marcar / Desmarcar Todos
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
  <div>

  
  </div>
</form>
<script language="javascript">
var k = 0;
var t = opener.document.formulario.coonid;
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
		if((opener.document.formulario.coonid.options.length == 1) && (opener.document.formulario.coonid.options[0].value == "")){
			opener.document.formulario.coonid.options[0] = null;
		}
	  	var d=opener.document.formulario.coonid.options.length++;
        opener.document.formulario.coonid.options[d].text = Descricao;
		opener.document.formulario.coonid.options[d].name = Nome;
		opener.document.formulario.coonid.options[d].value = Valor;
		opener.document.formulario.coonid.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.coonid.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.coonid.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.coonid.options.length++;
				opener.document.formulario.coonid.options[d].text = textocombogeral;
				opener.document.formulario.coonid.options[d].value = "";
				//opener.document.formulario.coonid.options[d].setAttribute("","");
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