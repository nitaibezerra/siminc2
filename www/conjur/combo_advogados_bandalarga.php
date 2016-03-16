<?php
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	

switch ( $_REQUEST['status'] ) {
  case 'A':
  case 'I':
    $PstStatus = $_REQUEST['status'];
    break;
  case 'T':
    $PstStatus = '';
    break;
  default:
    $PstStatus = 'A';
    break;
}
if( $PstStatus != '' ){
  $where = ($PstStatus) ? ("WHERE advstatus = '".$PstStatus."'") : "" ;
}

$sqlListaAdvogados="SELECT
                        a.advid AS codigo,
                        e.entnome AS descricao
                    FROM conjur.advogados a
                    INNER JOIN entidade.entidade e ON e.entid=a.entid AND entstatus='A'
                    ".$where."
                    ORDER BY descricao";

// ver($sqlListaAdvogados,d);
$listaAdvogados = $db->carregar( $sqlListaAdvogados );
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <tr>
      <td>Status:</td>
      <td>
        <select name="status" id="status">
              <?php 
                $arrStatus = array( array("codigo"=>"A","descricao"=>"Ativo"),
                                    array("codigo"=>"I","descricao"=>"Inativo"),
                                    array("codigo"=>"T", "descricao"=>"Todos") );

      	         foreach($arrStatus as $status){
                    $selected = "";
			              $ID = $status['codigo'];
			              $Nome = $status['descricao'];
                    if( $status['codigo'] == $PstStatus ){
                      $selected = "selected='selected'";
                    } ?>
                    <option id="opStatus" value="<?=$ID?>" <?=$selected ?>><?=$Nome?></option>
              <?php }?>

        </select>
        <input id="button" type="submit" name="button" value="Filtrar" />
      </td>
    </tr>
    <table width="500px" bordercolor="#DCDCDC">
      <tr><td style="background-color:#e9e9e9"><b>Selecione:</b></td><td style="background-color:#e9e9e9"><b>Advogados</b></td></tr>
      <?
      if(is_array($listaAdvogados)){
          foreach($listaAdvogados as $advogados):
              $Cor = "#e2e6e7 ";
              $Divisao=$Cont%2;
              ($Divisao == 0)? $Cor = "#fbfbfb " : "";
              $Cont = $Cont+1;

              ?>
              <tr>
                  <td style="background-color:<?=$Cor?>;">
                      <input name="checkbox<?= $advogados['codigo']?>" type="checkbox" title="<?= $advogados['descricao']?>" id="checkbox<?= $advogados['codigo']?>" value="<?= $advogados['codigo']?>"
                             onclick="obterMarcados('checkbox<?= $advogados['codigo']?>', '<?= $advogados['codigo']?>','<?= str_replace("'","",$advogados['descricao'])?>');" />
                  </td>
                  <td  style="background-color:<?=$Cor?>;"><?=$advogados['descricao']?> </td>

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
var t = opener.document.formulario.advid;
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
		if((opener.document.formulario.advid.options.length == 1) && (opener.document.formulario.advid.options[0].value == "")){
			opener.document.formulario.advid.options[0] = null;
		}
	  	var d=opener.document.formulario.advid.options.length++;
        opener.document.formulario.advid.options[d].text = Descricao;
		opener.document.formulario.advid.options[d].name = Nome;
		opener.document.formulario.advid.options[d].value = Valor;
		opener.document.formulario.advid.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.advid.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.advid.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.advid.options.length++;
				opener.document.formulario.advid.options[d].text = textocombogeral;
				opener.document.formulario.advid.options[d].value = "";
				//opener.document.formulario.advid.options[d].setAttribute("","");
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