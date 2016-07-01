<?php
    # Inicializa sistema
    require_once "config.inc";
    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    $db = new cls_banco();

    $nomeCombo = !empty($_REQUEST['nomeCombo'])?$_REQUEST['nomeCombo']:'municipios';
    $nomeFormulario = !empty($_REQUEST['nomeFormulario'])?$_REQUEST['nomeFormulario']:'formulario';
    
    $sqlEstado = "
        select
            estuf as codigo,
            estdescricao as descricao
        from territorios.estado
        order by
            estdescricao
    ";
    
    $estados = $db->carregar( $sqlEstado );
    $PstEstados = $_REQUEST["estados"];
    $mundescricao = $_REQUEST['mundescricao'];
    
    $where = array();
    if(!empty($PstEstados)){
        $where[] = " estuf = '".$PstEstados."' ";
    }

    if(!empty($mundescricao)){
        $where[] = " mundescricao ILIKE('%".pg_escape_string($mundescricao)."%') ";
    }

    $sqlListaMunicipios="
        SELECT DISTINCT
            muncod AS codigo,
            estuf AS estados,
            mundescricao AS nome
        FROM
            territorios.municipio
        ".(!empty($where)? ' WHERE ': NULL)."
            ". join(' AND ',$where)."
        ORDER BY
            mundescricao ASC";
    $municipios = $db->carregar( $sqlListaMunicipios );

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <tr>
        <td>Estados:</td>
        <td>
            <select name="estados" id="estados" class="link">
              <?php
                    foreach($estados as $estados){
                      $marcado ='';
                      $ID = $estados['codigo'];
                      $Nome = $estados['descricao'];

                      if($PstEstados == $ID){
                          $marcado = ' selected="selected" ';
                      }
              ?>
                <option value="<?=$ID?>" <?=$marcado;?> ><?=$Nome?></option>
              <?php
                    }
              ?>
            </select>
        </td>
    </tr>
    <tr>
        <td>Nome do município:</td>
        <td>
            <?php echo campo_texto('mundescricao', 'N', 'S', '', 20, 20, '', '', 'left', '', 0, '', '', $mundescricao); ?>
            <input id="button" type="submit" name="button" value="Filtrar" class="link" />
        </td>
    </tr>
    <table>
    <table width="100%" border="0">
    <?php
        if(!empty($municipios)):
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
                title="<?= $municipios['estados']." - ".$municipios['nome']?>" 
                id="checkbox<?= $municipios['codigo']?>" 
                value="<?= $municipios['codigo']?>"
                onclick="obterMarcados('checkbox<?= $municipios['codigo']?>', '<?= $municipios['codigo']?>','<?= str_replace("'","",$municipios['nome'])." - ".$municipios['estados']?>');" />
            </td>
            <td  style="background-color:<?=$Cor?>;"><?=$municipios['estados']?> - <?=$municipios['nome']?> </td>
        </tr>
        <?php endforeach; ?>
        <tr>
            <td>
                <input id="marcaDesmarcaTodos" type="checkbox" name="marcaDesmarcaTodos" value="marcaDesmarcaTodos" onclick="marcarDesmarcarTodos(this);"  /> Marcar / Desmarcar Todos
            </td>
        </tr>
    <?php else: ?>
        <tr>
            <td style="background-color:#e2e6e7; text-align: center;">
                Nenhum registro encontrado!
            </td>
        </tr>
    <?php endif; ?>
</table>
</form>
<script language="javascript">
    var k = 0;
    var t = opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>;
    var a = document.<?php echo $nomeFormulario; ?>.elements;
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
                    if((opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options.length == 1) && (opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[0].value == "")){
                            opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[0] = null;
                    }
                    var d=opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options.length++;
                    opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[d].text = Estado;
                    opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[d].value = Valor;
                    opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[d].setAttribute("selected","selected");
            }else{
                    var listaOpcoes = opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options;
                    for(x = 0 ; x< listaOpcoes.length; x++){
                            if(listaOpcoes[x].value == Valor ){
                                    opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[x] = null;
                            }
                            if(listaOpcoes.length == 0){
                                    var textocombogeral = "Duplo clique para selecionar da lista"; 
                                    var d=opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options.length++;
                                    opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[d].text = textocombogeral;
                                    opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[d].value = "";
                                    //opener.document.<?php echo $nomeFormulario; ?>.<?php echo $nomeCombo; ?>.options[d].setAttribute("","");
                            } 
                    }
            }   
    } 

    function marcarDesmarcarTodos(checkbox){
        for (i=0;i<document.<?php echo $nomeFormulario; ?>.elements.length;i++){
                if(document.<?php echo $nomeFormulario; ?>.elements[i].type == "checkbox"){
                if(checkbox.checked == true){
                    document.<?php echo $nomeFormulario; ?>.elements[i].checked=true;
                } else {
                    document.<?php echo $nomeFormulario; ?>.elements[i].checked=false;		 	
                }
                Nome = document.<?php echo $nomeFormulario; ?>.elements[i].name;
                Valor = document.<?php echo $nomeFormulario; ?>.elements[i].value;
                Estado = document.<?php echo $nomeFormulario; ?>.elements[i].title;
                obterMarcados( Nome, Valor,Estado);
            }
        }
    }

    function selecionaTodos(){
        for (i=0;i<document.<?php echo $nomeFormulario; ?>.elements.length;i++){ 
            if(document.<?php echo $nomeFormulario; ?>.elements[i].type == "checkbox"){
                document.<?php echo $nomeFormulario; ?>.elements[i].checked=true;
                Nome = document.<?php echo $nomeFormulario; ?>.elements[i].name;
                Valor = document.<?php echo $nomeFormulario; ?>.elements[i].value;
                Estado = document.<?php echo $nomeFormulario; ?>.elements[i].title;
                obterMarcados( Nome, Valor,Estado);
            }
        }
    }
</script>