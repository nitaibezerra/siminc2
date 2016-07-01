
<?php 

    $sql = "SELECT merid, mermantidaindicada1, mermantidaindicada2, mermantidaindicada3, 
			       merinexistsupervinstitucional, 
			       usucpf, merdtinclusao, merstatus
  			FROM maismedicomec.mantenedoraexpregulatoria
  			WHERE merstatus = 'A' and mntid = ".$_SESSION['maismedicomec']['mntid'];
    $dados = $db->carregar($sql);
    if($dados[0]) extract($dados[0]);
    
    //recupera muncod mantenedora
    $sql = "SELECT muncod
  			FROM maismedicomec.mantenedora
  			WHERE mntid = ".$_SESSION['maismedicomec']['mntid'];
    $muncodMantenedora = $db->pegaUm($sql);


    monta_titulo( 'Mantenedora - Avaliação Fase II', '' );
?>


<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>


<script type="text/javascript">

    function resetFromulario(){
        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'radio' ){
                $(v).attr('checked', false);
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }
        });
    }
    
    function resetFromulario2(){
        location.href="maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A";
    }

    function salvarDadosExpRegulatoria(){
        var erro;
        var radio_entro;
        var check_entro;
        var campos = '';
		/*
        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val() && (name != radio_entro) ){
                        erro = 1;
                        radio_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val() && (name != check_entro) ){
                        erro = 1;
                        check_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }
        });
		*/

		if($("#mermantidaindicada1").val() != '' && $("#mermantidaindicada2").val() != ''){
			if($("#mermantidaindicada1").val() == $("#mermantidaindicada2").val()){
				alert('Mantida 1 deve ser diferente da Mantida 2');
            	return false;
			}
		}
		
		if($("#mermantidaindicada1").val() != '' && $("#mermantidaindicada3").val() != ''){
			if($("#mermantidaindicada1").val() == $("#mermantidaindicada3").val()){
				alert('Mantida 1 deve ser diferente da Mantida 3');
            	return false;
			}
		}

		if($("#mermantidaindicada2").val() != '' && $("#mermantidaindicada3").val() != ''){
			if($("#mermantidaindicada2").val() == $("#mermantidaindicada3").val()){
				alert('Mantida 2 deve ser diferente da Mantida 3');
            	return false;
			}
		}
		
        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosExpRegulatoria');
            $('#formulario').submit();
        }
    }


    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }
    
    function addCampo(id){
        var ct = 0;
        ct = document.getElementById(id).value;
        document.getElementById(id).value = parseInt(ct) + 1;
        document.formulario.submit();
    }
    
    function removeCampo(id){
        var ct = 0;
        ct = document.getElementById(id).value;
        document.getElementById(id).value = parseInt(ct) - 1;
        document.formulario.submit();
    }
    
    function verificaCampo(contador, campo, mantida){
        
        var total = 0;
        
        if( campo == 'mercursomedicina' || campo == 'mercursosaude' ){
        	total = $("select[id^='"+campo+"']").length;
        }else{
        	total = $("input[id^='"+campo+"']").length;
        }
        
        if( mantida == '1' && $("#mermantidaindicada1").val() == '' ){
        	alert("Informe a Mantida 1.");
        	$("#mermantidaindicada1").focus();
        	return false; 
        }
        if( mantida == '2' && $("#mermantidaindicada2").val() == '' ){
        	alert("Informe a Mantida 2.");
        	$("#mermantidaindicada2").focus();
        	return false; 
        }
        if( mantida == '3' && $("#mermantidaindicada3").val() == '' ){
        	alert("Informe a Mantida 3.");
        	$("#mermantidaindicada3").focus();
        	return false; 
        }
        
        if( campo == 'mercursomedicina' && total > 2 ){
        	alert("É permitido somente 3 cursos de medicina para as 3 mantidas");
        	return false; 
        }
        
        if( campo == 'mercursosaude' && total > 2 ){
        	alert("É permitido somente 3 cursos na área de saúde para as 3 mantidas");
        	return false; 
        }

        if( campo == 'merresidencia' && total > 4 ){
        	alert("É permitido somente 5 programas residência para as 3 mantidas");
        	return false; 
        }

        if( campo == 'mermestrado' && total > 4 ){
        	alert("É permitido somente 5 mestrado/doutorado para as 3 mantidas");
        	return false; 
        }
        
        addCampo(contador);
        
    }
    
    function vMantida(id){
    	if(id == 1){
    		if($("#mermantidaindicada1").val() != ''){
				if($("#mermantidaindicada1").val() == $("#mermantidaindicada2").val() || $("#mermantidaindicada1").val() == $("#mermantidaindicada3").val()){
					alert('Mantida 1 deve ser diferente da Mantida 2 ou 3');
					$("#mermantidaindicada1").val('');
	            	return false;
				}
			}else{
				$('#formulario').submit();
			}
    	}
    	if(id == 2){
    		if($("#mermantidaindicada2").val() != ''){
				if($("#mermantidaindicada2").val() == $("#mermantidaindicada1").val() || $("#mermantidaindicada2").val() == $("#mermantidaindicada3").val()){
					alert('Mantida 2 deve ser diferente da Mantida 1 ou 3');
					$("#mermantidaindicada2").val('');
	            	return false;
				}
			}else{
				$('#formulario').submit();
			}
    	}
    	if(id == 3){
    		if($("#mermantidaindicada3").val() != ''){
				if($("#mermantidaindicada3").val() == $("#mermantidaindicada1").val() || $("#mermantidaindicada3").val() == $("#mermantidaindicada2").val()){
					alert('Mantida 1 deve ser diferente da Mantida 1 ou 2');
					$("#mermantidaindicada3").val('');
	            	return false;
				}
			}else{
				$('#formulario').submit();
			}
    	}
    }
    
</script>



<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

	<input type="hidden" id="submite" name="submite" value="ok"/>
    <input type="hidden" id="merid" name="merid" value="<?=$merid;?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
<!--        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Pertence ao Corpo Dirigente </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Faz parte de qual Corpo Dirigente: </td>
            <td colspan="2">
                
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>-->
        <tr>
            <td class ="SubTituloDireita" width="35%">Mantida 1: </td>
            <td colspan="2">
                <?PHP
					if($_POST['submite']){
						$mermantidaindicada1 = $_POST['mermantidaindicada1'];
					}else{
						$mermantidaindicada1 = $mermantidaindicada1;
					}
					
					/*
					$sql = "
                        SELECT  m.mtdid AS codigo,
                                m.mtddsc AS descricao
                        FROM maismedicomec.mantida AS m
                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                        ORDER BY descricao
                    ";
					 */
                    $sql = "
                        SELECT  m.iesid AS codigo,
                                m.iesdsc AS descricao
                        FROM gestaodocumentos.instituicaoensino AS m
                        WHERE m.mntid = {$_SESSION['maismedicomec']['mntid']}
                        and m.iesid not in (select mtdid 
                        					from maismedicomec.mantidamunicipio
                        					where mntid = {$_SESSION['maismedicomec']['mntid']}
                        					)
                        ORDER BY 2
                    ";
                    
                    $db->monta_combo('mermantidaindicada1', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mermantidaindicada1', '', $mermantidaindicada1, 'Mantida Indicada 1', 'onchange="vMantida(1);"');
                    
                    
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar curso de medicina: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctcursomedicina1', 'mercursomedicina', '1');"></td>
            <td colspan="2">
                <?
                if($mermantidaindicada1){
                	$sql = "select crmid from maismedicomec.mantenedoraareamedica 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada1}
                			order by mamid";
                	$dadosMedicina1 = $db->carregar($sql);
                	if(!$dadosMedicina1){
                		$totalMedicina1 = 0;
                	}else{
                		$totalMedicina1 = count($dadosMedicina1);
                	}
                }else{
                	$totalMedicina1 = 0;
                	$_POST['ctcursomedicina1'] = 0;
                }
                
                
				$ctcursomedicina1 = $_POST['ctcursomedicina1'] ? $_POST['ctcursomedicina1'] : $totalMedicina1;
				
				if($ctcursomedicina1==0) echo 'Nenhum curso adicionado.';
				
				for($i=1; $i<=$ctcursomedicina1; $i++){
					/*
					$sql = "
	                        SELECT  m.crmid AS codigo,
	                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
	                        FROM maismedicomec.cursomedicina AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
	                        ORDER BY descricao";
					*/
					if($mermantidaindicada1){
						$sql = "SELECT  min(m.crmid) AS codigo,
		                                m.crmdsccurso AS descricao
		                        FROM maismedicomec.cursomedicina AS m
		                        WHERE m.iesid = {$mermantidaindicada1}
		                        group by m.crmdsccurso
		                        ORDER BY 2";
					}
					
					$mercursomedicina1 = "";
					if($_POST['submite']){
						$mercursomedicina1 = $_POST['mercursomedicina1'][($i-1)];
					}else{
						$mercursomedicina1 = $dadosMedicina1[($i-1)];
					}
					
	                $db->monta_combo('mercursomedicina1[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursomedicina1', '', $mercursomedicina1, 'Curso de medicina');
	                
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursomedicina1\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctcursomedicina1" id="ctcursomedicina1" value="<?=$ctcursomedicina1?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar curso área de saúde: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctcursosaude1', 'mercursosaude', '1');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada1){
                	$sql = "select casid from maismedicomec.mantenedoraareasaude 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada1}
                			order by masid";
                	$dadosSaude1 = $db->carregar($sql);
                	if(!$dadosSaude1){
                		$totalSaude1 = 0;
                	}else{
                		$totalSaude1 = count($dadosSaude1);
                	}
                }else{
                	$totalSaude1 = 0;
                	$_POST['ctcursosaude1'] = 0;
                }
                
				$ctcursosaude1 = $_POST['ctcursosaude1'] ? $_POST['ctcursosaude1'] : $totalSaude1;
				
				if($ctcursosaude1==0) echo 'Nenhum curso adicionado.';
				
				for($i=1; $i<=$ctcursosaude1; $i++){
					/*
					$sql = "
	                        SELECT  m.casid AS codigo,
	                                m.cascodcurso||' - '||m.casdsccurso AS descricao
	                        FROM maismedicomec.cursoareasaude AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
	                        ORDER BY descricao
	                    ";
					*/
					
					if($mermantidaindicada1){
						$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        WHERE m.iesid = {$mermantidaindicada1} 
		                        group by m.casdsccurso
		                        ORDER BY 2";
					}
					
					
					$mercursosaude1 = "";
					if($_POST['submite']){
						$mercursosaude1 = $_POST['mercursosaude1'][($i-1)];
					}else{
						$mercursosaude1 = $dadosSaude1[($i-1)];
					}
					
	                $db->monta_combo('mercursosaude1[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursosaude1', '', $mercursosaude1, 'Curso de saúde');
	                
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursosaude1\');">';
	               	} 
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctcursosaude1" id="ctcursosaude1" value="<?=$ctcursosaude1?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar programa residência: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctresidencia1', 'merresidencia', '1');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada1){
                	$sql = "select mprdsc from maismedicomec.mantenedoraprogresidencia 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada1}
                			order by mprid";
                	$dadosResidencia1 = $db->carregarColuna($sql);
                	if(!$dadosResidencia1){
                		$totalResidencia1 = 0;
                	}else{
                		$totalResidencia1 = count($dadosResidencia1);
                	}
                }else{
                	$totalResidencia1 = 0;
                	$_POST['ctresidencia1'] = 0;
                }
                
				$ctresidencia1 = $_POST['ctresidencia1'] ? $_POST['ctresidencia1'] : $totalResidencia1;
				
				if($ctresidencia1==0) echo 'Nenhum programa adicionado.';
				
				for($i=1; $i<=$ctresidencia1; $i++){
					
					$merresidencia1 = "";
					if($_POST['submite']){
						$merresidencia1 = $_POST['merresidencia1'][($i-1)];
					}else{
						$merresidencia1 = $dadosResidencia1[($i-1)];
					}
					
	                echo campo_texto('merresidencia1[]', 'N', 'S', 'Programa Residência', 59, 100, '', '', '', '', 0, 'id="merresidencia1"', '', $merresidencia1, '', null);

	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctresidencia1\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctresidencia1" id="ctresidencia1" value="<?=$ctresidencia1?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar mestrado/doutorado: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctmestrado1', 'mermestrado', '1');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada1){
                	$sql = "select mpmdsc from maismedicomec.mantenedoraprogmestdoutorado 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada1}
                			order by mpmid";
                	$dadosMestrado1 = $db->carregarColuna($sql);
                	if(!$dadosMestrado1){
                		$totalMestrado1 = 0;
                	}else{
                		$totalMestrado1 = count($dadosMestrado1);
                	}
                }else{
                	$totalMestrado1 = 0;
                	$_POST['ctmestrado1'] = 0;
                }
                
				$ctmestrado1 = $_POST['ctmestrado1'] ? $_POST['ctmestrado1'] : $totalMestrado1;
				
				if($ctmestrado1==0) echo 'Nenhum mestrado/doutorado adicionado.';
				
				for($i=1; $i<=$ctmestrado1; $i++){
					
					$mermestrado1 = "";
					if($_POST['submite']){
						$mermestrado1 = $_POST['mermestrado1'][($i-1)];
					}else{
						$mermestrado1 = $dadosMestrado1[($i-1)];
					}
					
	                echo campo_texto('mermestrado1[]', 'N', 'S', 'Programa mestrado/doutorado', 59, 100, '', '', '', '', 0, 'id="mermestrado1"', '', $mermestrado1, '', null);

	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctmestrado1\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctmestrado1" id="ctmestrado1" value="<?=$ctmestrado1?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" colspan="2">&nbsp;</td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Mantida 2: </td>
            <td colspan="2">
                <?PHP
					if($_POST['submite']){
						$mermantidaindicada2 = $_POST['mermantidaindicada2'];
					}else{
						$mermantidaindicada2 = $mermantidaindicada2;
					}
					
                    $sql = "
                        SELECT  m.iesid AS codigo,
                                m.iesdsc AS descricao
                        FROM gestaodocumentos.instituicaoensino AS m
                        WHERE m.mntid = {$_SESSION['maismedicomec']['mntid']}
                        and m.iesid not in (select mtdid 
                        					from maismedicomec.mantidamunicipio
                        					where mntid = {$_SESSION['maismedicomec']['mntid']}
                        					)
                        ORDER BY 2
                    ";
                    
                    $db->monta_combo('mermantidaindicada2', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mermantidaindicada2', '', $mermantidaindicada2, 'Mantida Indicada 2', 'onchange="vMantida(2);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar curso de medicina: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctcursomedicina2', 'mercursomedicina', '2');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada2){
                	$sql = "select crmid from maismedicomec.mantenedoraareamedica 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada2}
                			order by mamid";
                	$dadosMedicina2 = $db->carregar($sql);
                	if(!$dadosMedicina2){
                		$totalMedicina2 = 0;
                	}else{
                		$totalMedicina2 = count($dadosMedicina2);
                	}
                }else{
                	$totalMedicina2 = 0;
                	$_POST['ctcursomedicina2'] = 0;
                }
                
				$ctcursomedicina2 = $_POST['ctcursomedicina2'] ? $_POST['ctcursomedicina2'] : $totalMedicina2;
				
				if($ctcursomedicina2==0) echo 'Nenhum curso adicionado.';
				
				for($i=1; $i<=$ctcursomedicina2; $i++){
					/*
					$sql = "
	                        SELECT  m.crmid AS codigo,
	                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
	                        FROM maismedicomec.cursomedicina AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
	                        ORDER BY descricao
	                    ";
					*/
					if($mermantidaindicada2){
						$sql = "SELECT  min(m.crmid) AS codigo,
		                                m.crmdsccurso AS descricao
		                        FROM maismedicomec.cursomedicina AS m
		                        WHERE m.iesid = {$mermantidaindicada2}
		                        group by m.crmdsccurso
		                        ORDER BY 2";
					}
					
					$mercursomedicina2 = "";
					if($_POST['submite']){
						$mercursomedicina2 = $_POST['mercursomedicina2'][($i-1)];
					}else{
						$mercursomedicina2 = $dadosMedicina2[($i-1)];
					}
					
					
	                $db->monta_combo('mercursomedicina2[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursomedicina2', '', $mercursomedicina2, 'Curso de medicina');
	                
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursomedicina2\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctcursomedicina2" id="ctcursomedicina2" value="<?=$ctcursomedicina2?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar curso área de saúde: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctcursosaude2', 'mercursosaude', '2');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada2){
                	$sql = "select casid from maismedicomec.mantenedoraareasaude 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada2}
                			order by masid";
                	$dadosSaude2 = $db->carregar($sql);
                	if(!$dadosSaude2){
                		$totalSaude2 = 0;
                	}else{
                		$totalSaude2 = count($dadosSaude2);
                	}
                }else{
                	$totalSaude2 = 0;
                	$_POST['ctcursosaude2'] = 0;
                }
                
				$ctcursosaude2 = $_POST['ctcursosaude2'] ? $_POST['ctcursosaude2'] : $totalSaude2;
				
				if($ctcursosaude2==0) echo 'Nenhum curso adicionado.';
				
				for($i=1; $i<=$ctcursosaude2; $i++){
					/*
					$sql = "
	                        SELECT  m.casid AS codigo,
	                                m.cascodcurso||' - '||m.casdsccurso AS descricao
	                        FROM maismedicomec.cursoareasaude AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
	                        ORDER BY descricao
	                    ";
					*/
					
					if($mermantidaindicada2){
						$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        WHERE m.iesid = {$mermantidaindicada2} 
		                        group by m.casdsccurso
		                        ORDER BY 2";
					}
					
					$mercursosaude2 = "";
					if($_POST['submite']){
						$mercursosaude2 = $_POST['mercursosaude2'][($i-1)];
					}else{
						$mercursosaude2 = $dadosSaude2[($i-1)];
					}
					
					
	                $db->monta_combo('mercursosaude2[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursosaude2', '', $mercursosaude2, 'Curso de saúde');

	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursosaude2\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctcursosaude2" id="ctcursosaude2" value="<?=$ctcursosaude2?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar programa residência: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctresidencia2', 'merresidencia', '2');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada2){
                	$sql = "select mprdsc from maismedicomec.mantenedoraprogresidencia 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada2}
                			order by mprid";
                	$dadosResidencia2 = $db->carregarColuna($sql);
                	if(!$dadosResidencia2){
                		$totalResidencia2 = 0;
                	}else{
                		$totalResidencia2 = count($dadosResidencia2);
                	}
                }else{
                	$totalResidencia2 = 0;
                	$_POST['ctresidencia2'] = 0;
                }
                
				$ctresidencia2 = $_POST['ctresidencia2'] ? $_POST['ctresidencia2'] : $totalResidencia2;
				
				if($ctresidencia2==0) echo 'Nenhum programa adicionado.';
				
				for($i=1; $i<=$ctresidencia2; $i++){
					
					
					$merresidencia2 = "";
					if($_POST['submite']){
						$merresidencia2 = $_POST['merresidencia2'][($i-1)];
					}else{
						$merresidencia2 = $dadosResidencia2[($i-1)];
					}
					
					
	                echo campo_texto('merresidencia2[]', 'N', 'S', 'Programa Residência', 59, 100, '', '', '', '', 0, 'id="merresidencia2"', '', $merresidencia2, '', null);
	                 
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctresidencia2\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctresidencia2" id="ctresidencia2" value="<?=$ctresidencia2?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar mestrado/doutorado: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctmestrado2', 'mermestrado', '2');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada2){
                	$sql = "select mpmdsc from maismedicomec.mantenedoraprogmestdoutorado 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada2}
                			order by mpmid";
                	$dadosMestrado2 = $db->carregarColuna($sql);
                	if(!$dadosMestrado2){
                		$totalMestrado2 = 0;
                	}else{
                		$totalMestrado2 = count($dadosMestrado2);
                	}
                }else{
                	$totalMestrado2 = 0;
                	$_POST['ctmestrado2'] = 0;
                }
                
				$ctmestrado2 = $_POST['ctmestrado2'] ? $_POST['ctmestrado2'] : $totalMestrado2;
				
				if($ctmestrado2==0) echo 'Nenhum mestrado/doutorado adicionado.';
				
				for($i=1; $i<=$ctmestrado2; $i++){
					
					$mermestrado2 = "";
					if($_POST['submite']){
						$mermestrado2 = $_POST['mermestrado2'][($i-1)];
					}else{
						$mermestrado2 = $dadosMestrado2[($i-1)];
					}
					
	                echo campo_texto('mermestrado2[]', 'N', 'S', 'Programa mestrado/doutorado', 59, 100, '', '', '', '', 0, 'id="mermestrado2"', '', $mermestrado2, '', null);
	                
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctmestrado2\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctmestrado2" id="ctmestrado2" value="<?=$ctmestrado2?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" colspan="2">&nbsp;</td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Mantida 3: </td>
            <td colspan="2">
                <?PHP
					if($_POST['submite']){
						$mermantidaindicada3 = $_POST['mermantidaindicada3'];
					}else{
						$mermantidaindicada3 = $mermantidaindicada3;
					}
					
                    $sql = "
                        SELECT  m.iesid AS codigo,
                                m.iesdsc AS descricao
                        FROM gestaodocumentos.instituicaoensino AS m
                        WHERE m.mntid = {$_SESSION['maismedicomec']['mntid']}
                        and m.iesid not in (select mtdid 
                        					from maismedicomec.mantidamunicipio
                        					where mntid = {$_SESSION['maismedicomec']['mntid']}
                        					)
                        ORDER BY 2
                    ";
                    
                    $db->monta_combo('mermantidaindicada3', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mermantidaindicada3', '', $mermantidaindicada3, 'Mantida Indicada 3', 'onchange="vMantida(3);"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar curso de medicina: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctcursomedicina3', 'mercursomedicina', '3');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada3){
                	$sql = "select crmid from maismedicomec.mantenedoraareamedica 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada3}
                			order by mamid";
                	$dadosMedicina3 = $db->carregar($sql);
                	if(!$dadosMedicina3){
                		$totalMedicina3 = 0;
                	}else{
                		$totalMedicina3 = count($dadosMedicina3);
                	}
                }else{
                	$totalMedicina3 = 0;
                	$_POST['ctcursomedicina3'] = 0;
                }
                
				$ctcursomedicina3 = $_POST['ctcursomedicina3'] ? $_POST['ctcursomedicina3'] : $totalMedicina3;
				
				if($ctcursomedicina3==0) echo 'Nenhum curso adicionado.';
				
				for($i=1; $i<=$ctcursomedicina3; $i++){
					/*
					$sql = "
	                        SELECT  m.crmid AS codigo,
	                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
	                        FROM maismedicomec.cursomedicina AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
	                        ORDER BY descricao
	                    ";
					*/
					
					if($mermantidaindicada3){
						$sql = "SELECT  min(m.crmid) AS codigo,
		                                m.crmdsccurso AS descricao
		                        FROM maismedicomec.cursomedicina AS m
		                        WHERE m.iesid = {$mermantidaindicada3}
		                        group by m.crmdsccurso
		                        ORDER BY 2";
					}
					
					$mercursomedicina3 = "";
					if($_POST['submite']){
						$mercursomedicina3 = $_POST['mercursomedicina3'][($i-1)];
					}else{
						$mercursomedicina3 = $dadosMedicina3[($i-1)];
					}
					
					
	                $db->monta_combo('mercursomedicina3[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursomedicina3', '', $mercursomedicina3, 'Curso de medicina');
	                
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursomedicina3\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctcursomedicina3" id="ctcursomedicina3" value="<?=$ctcursomedicina3?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar curso área de saúde: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctcursosaude3', 'mercursosaude', '3');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada3){
                	$sql = "select casid from maismedicomec.mantenedoraareasaude 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada3}
                			order by masid";
                	$dadosSaude3 = $db->carregar($sql);
                	if(!$dadosSaude3){
                		$totalSaude3 = 0;
                	}else{
                		$totalSaude3 = count($dadosSaude3);
                	}
                }else{
                	$totalSaude3 = 0;
                	$_POST['ctcursosaude3'] = 0;
                }
                
				$ctcursosaude3 = $_POST['ctcursosaude3'] ? $_POST['ctcursosaude3'] : $totalSaude3;
				
				if($ctcursosaude3==0) echo 'Nenhum curso adicionado.';
				
				for($i=1; $i<=$ctcursosaude3; $i++){
					/*
					$sql = "
	                        SELECT  m.casid AS codigo,
	                                m.cascodcurso||' - '||m.casdsccurso AS descricao
	                        FROM maismedicomec.cursoareasaude AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
	                        ORDER BY descricao
	                    ";
					*/
					
					if($mermantidaindicada3){
						$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        WHERE m.iesid = {$mermantidaindicada3} 
		                        group by m.casdsccurso
		                        ORDER BY 2";
					}
					
					$mercursosaude3 = "";
					if($_POST['submite']){
						$mercursosaude3 = $_POST['mercursosaude3'][($i-1)];
					}else{
						$mercursosaude3 = $dadosSaude3[($i-1)];
					}
					
					
	                $db->monta_combo('mercursosaude3[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursosaude3', '', $mercursosaude3, 'Curso de saúde');
	                
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursosaude3\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctcursosaude3" id="ctcursosaude3" value="<?=$ctcursosaude3?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar programa residência: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctresidencia3', 'merresidencia', '3');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada3){
                	$sql = "select mprdsc from maismedicomec.mantenedoraprogresidencia 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada3}
                			order by mprid";
                	$dadosResidencia3 = $db->carregarColuna($sql);
                	if(!$dadosResidencia3){
                		$totalResidencia3 = 0;
                	}else{
                		$totalResidencia3 = count($dadosResidencia3);
                	}
                }else{
                	$totalResidencia3 = 0;
                	$_POST['ctresidencia3'] = 0;
                }
                
				$ctresidencia3 = $_POST['ctresidencia3'] ? $_POST['ctresidencia3'] : $totalResidencia3;
				
				if($ctresidencia3==0) echo 'Nenhum programa adicionado.';
				
				for($i=1; $i<=$ctresidencia3; $i++){
					
					$merresidencia3 = "";
					if($_POST['submite']){
						$merresidencia3 = $_POST['merresidencia3'][($i-1)];
					}else{
						$merresidencia3 = $dadosResidencia3[($i-1)];
					}
					
					
	                echo campo_texto('merresidencia3[]', 'N', 'S', 'Programa Residência', 59, 100, '', '', '', '', 0, 'id="merresidencia3"', '', $merresidencia3, '', null);
	                 
	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctresidencia3\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctresidencia3" id="ctresidencia3" value="<?=$ctresidencia3?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Adicionar mestrado/doutorado: <img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom; cursor: hand" onclick="verificaCampo('ctmestrado3', 'mermestrado', '3');"></td>
            <td colspan="2">
                <?
				if($mermantidaindicada3){
                	$sql = "select mpmdsc from maismedicomec.mantenedoraprogmestdoutorado 
                			where mntid = {$_SESSION['maismedicomec']['mntid']}
                			and mtdid = {$mermantidaindicada3}
                			order by mpmid";
                	$dadosMestrado3 = $db->carregarColuna($sql);
                	if(!$dadosMestrado3){
                		$totalMestrado3 = 0;
                	}else{
                		$totalMestrado3 = count($dadosMestrado3);
                	}
                }else{
                	$totalMestrado3 = 0;
                	$_POST['ctmestrado3'] = 0;
                }
                
				$ctmestrado3 = $_POST['ctmestrado3'] ? $_POST['ctmestrado3'] : $totalMestrado3;
				
				if($ctmestrado3==0) echo 'Nenhum mestrado/doutorado adicionado.';
				
				for($i=1; $i<=$ctmestrado3; $i++){
					
					$mermestrado3 = "";
					if($_POST['submite']){
						$mermestrado3 = $_POST['mermestrado3'][($i-1)];
					}else{
						$mermestrado3 = $dadosMestrado3[($i-1)];
					}
					
					
	                echo campo_texto('mermestrado3[]', 'N', 'S', 'Programa mestrado/doutorado', 59, 100, '', '', '', '', 0, 'id="mermestrado3"', '', $mermestrado3, '', null);

	                if( !$boConsulta ){
	                	echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctmestrado3\');">';
	                }
	                echo '<br>';
					
				}
                
                ?>
                <input type="hidden" name="ctmestrado3" id="ctmestrado3" value="<?=$ctmestrado3?>">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" colspan="2">&nbsp;</td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Existência de supervisão institucional nos últimos dois anos? </td>
            <td colspan="2">
                <input type="radio" name="merinexistsupervinstitucional" value="t" <?if($merinexistsupervinstitucional == 't'){echo 'checked';}?>> SIM
                &nbsp;&nbsp;&nbsp;
                <input type="radio" name="merinexistsupervinstitucional" value="f" <?if($merinexistsupervinstitucional == 'f'){echo 'checked';}?>> NÃO
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td width="80%" class="SubTituloCentro" colspan="4" style="font-weight: bold">
            	<?PHP
            		$perfil = pegaPerfilGeral();
                	$docid = criaDocidMantenedora( $_SESSION['maismedicomec']['mntid'] );
                	$esdid = pegaEstadoAtualWorkflow( $docid );
                
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
	                    <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosExpRegulatoria();"/>
                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
                <?PHP
                    }elseif( in_array(PERFIL_IES, $perfil) && ($esdid == WF_EM_PREENCHIMENTO_IES || $esdid == WF_EM_AJUSTE_IES) ){
                    	if(date('Ymd') < 20150124){
	                    	?>
	                    	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosExpRegulatoria();"/>
	                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
	                    	<?
						}
                    }
                ?>
                
                 
                <input type="button" id="cancelar" name="voltar" value="Voltar" onclick="voltarPaginaPrincipal();"/>
                
            </td>
        </tr>
    </table>
</form>

