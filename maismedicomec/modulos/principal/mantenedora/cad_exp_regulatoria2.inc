<?PHP

	if($_REQUEST['mneid']){
	    $sql = "SELECT mneid, m.mtdid, mnecursomedicina, mneprogresidencia1, mneprogresidencia2, 
				       mneprogresidencia3, mneprogresidencia4, mneprogresidencia5, mneprogmestdoutorado1, 
				       mneprogmestdoutorado2, mneprogmestdoutorado3, mneprogmestdoutorado4, 
				       mneprogmestdoutorado5, mnecursoareasaude1, mnecursoareasaude2, 
				       mnecursoareasaude3, mneaderentefies, mneaderenteprouni, usucpf, 
				       mnedtinclusao, mnestatus
	  			FROM maismedicomec.mantidaexpregulatoria m
	  			--JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
	  			WHERE mntid = ".$_SESSION['maismedicomec']['mntid']."
	  			and mneid = ".$_REQUEST['mneid']."
	  			";
	    //dbg($sql,1);
	    $dados = $db->carregar($sql);
	    if($dados[0]) extract($dados[0]);
	}

    monta_titulo( 'Mantida Indicada - Avaliação Fase II', '' );
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
       location.href="maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantida";
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
		
		if(document.formulario.mtdid.value==''){
			alert("Informe a Mantida.");
			document.formulario.mtdid.focus();
            return false;
		}
		
        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosExpRegulatoria2');
            $('#formulario').submit();
        }
    }
    
    function buscarDadosEditarMantidaExpReg( mneid ){
        
        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantida&mneid='+mneid;
    }
    
     function excluirDadosMantidaExpReg( mneid ){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
        	window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantida&requisicao=excluirDadosMantidaExpReg&mneid='+mneid;
        }
    }

	
    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }
    
</script>



<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <input type="hidden" id="mneid" name="mneid" value="<?=$mneid;?>"/>

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
            <td class ="SubTituloDireita">Mantida Indicada: </td>
            <td colspan="2">
                <?PHP
                	$mtdid = $_POST['mtdid'] ? $_POST['mtdid'] : $mtdid;
                    
                	$sql = "
                        SELECT distinct  m.mtdid AS codigo,
                                m.mtddsc AS descricao
                        FROM maismedicomec.mantida AS m
                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                        ORDER BY 2
                    ";
					
                	/*
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
					*/
                    $db->monta_combo('mtdid', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mtdid', '', $mtdid, 'Mantida Indicada', 'onchange="document.formulario.submit();"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Curso de medicina e-mec: </td>
            <td colspan="2">
                <?PHP
                	if($mtdid){
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
                		/*
                		$sql = "
		                        SELECT  m.crmid AS codigo,
		                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
		                        FROM maismedicomec.cursomedicina AS m
		                        
		                        WHERE m.iesid = {$mtdid}
		                        ORDER BY descricao";
						*/
                		$sql = "SELECT  min(m.crmid) AS codigo,
		                                m.crmdsccurso AS descricao
		                        FROM maismedicomec.cursomedicina AS m
		                        WHERE m.iesid = {$mtdid}
		                        group by m.crmdsccurso
		                        ORDER BY 2";
	                    $db->monta_combo('mnecursomedicina', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mnecursomedicina', '', $mnecursomedicina, 'Curso de medicina');
                	}else{
                		echo 'Informe a Mantida.';
                	}
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Residência 1: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogresidencia1', 'N', 'S', 'Programas Residência 1', 59, 100, '', '', '', '', 0, 'id="mneprogresidencia1"', '', $mneprogresidencia1, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Residência 2: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogresidencia2', 'N', 'S', 'Programas Residência 2', 59, 100, '', '', '', '', 0, 'id="mneprogresidencia2"', '', $mneprogresidencia2, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Residência 3: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogresidencia3', 'N', 'S', 'Programas Residência 3', 59, 100, '', '', '', '', 0, 'id="mneprogresidencia3"', '', $mneprogresidencia3, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Residência 4: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogresidencia4', 'N', 'S', 'Programas Residência 4', 59, 100, '', '', '', '', 0, 'id="mneprogresidencia4"', '', $mneprogresidencia4, '', null);
                ?>
            </td>
        </tr>
         <tr>
            <td class="SubTituloDireita" width="30%"> Programas Residência 5: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogresidencia5', 'N', 'S', 'Programas Residência 5', 59, 100, '', '', '', '', 0, 'id="mneprogresidencia5"', '', $mneprogresidencia5, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Mestrado/doutorado 1: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogmestdoutorado1', 'N', 'S', 'Programas Mestrado/doutorado 1', 59, 100, '', '', '', '', 0, 'id="mneprogmestdoutorado1"', '', $mneprogmestdoutorado1, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Mestrado/doutorado 2: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogmestdoutorado2', 'N', 'S', 'Programas Mestrado/doutorado 2', 59, 100, '', '', '', '', 0, 'id="mneprogmestdoutorado2"', '', $mneprogmestdoutorado2, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Mestrado/doutorado 3: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogmestdoutorado3', 'N', 'S', 'Programas Mestrado/doutorado 3', 59, 100, '', '', '', '', 0, 'id="mneprogmestdoutorado3"', '', $mneprogmestdoutorado3, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%"> Programas Mestrado/doutorado 4: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogmestdoutorado4', 'N', 'S', 'Programas Mestrado/doutorado 4', 59, 100, '', '', '', '', 0, 'id="mneprogmestdoutorado4"', '', $mneprogmestdoutorado4, '', null);
                ?>
            </td>
        </tr>
         <tr>
            <td class="SubTituloDireita" width="30%"> Programas Mestrado/doutorado 5: </td>
            <td colspan="2">
                <?PHP
                    echo campo_texto('mneprogmestdoutorado5', 'N', 'S', 'Programas Mestrado/doutorado 5', 59, 100, '', '', '', '', 0, 'id="mneprogmestdoutorado5"', '', $mneprogmestdoutorado5, '', null);
                ?>
            </td>
        </tr>
        

        <tr>
            <td class ="SubTituloDireita">Curso área de saúde 1: </td>
            <td colspan="2">
                <?PHP
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
					if($mtdid){
						/*
						$sql = "
		                        SELECT  m.casid AS codigo,
		                                m.cascodcurso||' - '||m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        
		                        WHERE m.iesid = {$mtdid}
		                        ORDER BY descricao";
						*/
						$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        WHERE m.iesid = {$mtdid} 
		                        group by m.casdsccurso
		                        ORDER BY 2";
						//dbg($sql);
						$db->monta_combo('mnecursoareasaude1', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mnecursoareasaude1', '', $mnecursoareasaude1, 'Curso área de saúde 1');
					}else{
						echo 'Informe a Mantida.';
					}
                    
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Curso área de saúde 2: </td>
            <td colspan="2">
                <?PHP
                	/*
                    $sql = "
                        SELECT  m.casid AS codigo,
                                m.cascodcurso||' - '||m.casdsccurso AS descricao
                        FROM maismedicomec.cursoareasaude AS m
                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                        ORDER BY descricao
                    ";
                    $db->monta_combo('mnecursoareasaude2', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mnecursoareasaude2', '', $mnecursoareasaude2, 'Curso área de saúde 2');
					*/
                
					if($mtdid){
						$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        WHERE m.iesid = {$mtdid} 
		                        group by m.casdsccurso
		                        ORDER BY 2";
						$db->monta_combo('mnecursoareasaude2', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mnecursoareasaude2', '', $mnecursoareasaude2, 'Curso área de saúde 2');
					}else{
						echo 'Informe a Mantida.';
					}
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Curso área de saúde 3: </td>
            <td colspan="2">
                <?PHP
                	/*
                    $sql = "
                        SELECT  m.casid AS codigo,
                                m.cascodcurso||' - '||m.casdsccurso AS descricao
                        FROM maismedicomec.cursoareasaude AS m
                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
                        ORDER BY descricao
                    ";
                    $db->monta_combo('mnecursoareasaude3', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mnecursoareasaude3', '', $mnecursoareasaude3, 'Curso área de saúde 3');
					*/
					if($mtdid){
						$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
		                        FROM maismedicomec.cursoareasaude AS m
		                        WHERE m.iesid = {$mtdid} 
		                        group by m.casdsccurso
		                        ORDER BY 2";
						$db->monta_combo('mnecursoareasaude3', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mnecursoareasaude3', '', $mnecursoareasaude3, 'Curso área de saúde 3');
					}else{
						echo 'Informe a Mantida.';
					}
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Aderente ao FIES? </td>
            <td colspan="2">
                <input type="radio" name="mneaderentefies" value="t" <?if($mneaderentefies == 't'){echo 'checked';}?>> SIM
                &nbsp;&nbsp;&nbsp;
                <input type="radio" name="mneaderentefies" value="f" <?if($mneaderentefies == 'f'){echo 'checked';}?>> NÃO
            </td>
        </tr>
         <tr>
            <td class ="SubTituloDireita">Aderente ao PROUNI? </td>
            <td colspan="2">
                <input type="radio" name="mneaderenteprouni" value="t" <?if($mneaderenteprouni == 't'){echo 'checked';}?>> SIM
                &nbsp;&nbsp;&nbsp;
                <input type="radio" name="mneaderenteprouni" value="f" <?if($mneaderenteprouni == 'f'){echo 'checked';}?>> NÃO
            </td>
        </tr>
        
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td width="80%" class="SubTituloCentro" colspan="4" style="font-weight: bold">
            	
                <?PHP
                
	                //verifica total de mantidas
					$sql = "SELECT coalesce(count(m.mtdid),0) as total
	                        FROM maismedicomec.mantida AS m
	                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
	                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}";
	            	$total1 = $db->pegaUm($sql);
	            	
					$sql = "SELECT count(mneid) as total
		  			FROM maismedicomec.mantidaexpregulatoria m
		  			WHERE mnestatus = 'A' and mntid = ".$_SESSION['maismedicomec']['mntid'];
		    		$total2 = $db->pegaUm($sql);
	    		
	    		
            		$perfil = pegaPerfilGeral();
                	$docid = criaDocidMantenedora( $_SESSION['maismedicomec']['mntid'] );
                	$esdid = pegaEstadoAtualWorkflow( $docid );
                
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
                        <?if($total2 < $total1){?>
		                	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosExpRegulatoria();"/>
		                <?}else{?>
		                	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="alert('Não é possível gravar, pois já atingiu o limite máximo de mantidas');"/>
		                <?}?>

                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
                <?PHP
                    }elseif( in_array(PERFIL_IES, $perfil) && ($esdid == WF_EM_PREENCHIMENTO_IES || $esdid == WF_EM_AJUSTE_IES) ){
                    	if(date('Ymd') < 20150124){
	                    	?>
	                    	<?if($total2 < $total1){?>
			                	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosExpRegulatoria();"/>
			                <?}else{?>
			                	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="alert('Não é possível gravar, pois já atingiu o limite máximo de mantidas');"/>
			                <?}?>
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

<?PHP

	
    $acao = "
		        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarMantidaExpReg('||me.mneid||');\" title=\"Editar\" >
		        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosMantidaExpReg('||me.mneid||');\" title=\"Editar\" >
		    ";
    
	if( in_array(PERFIL_IES, $perfil) && ($esdid != WF_EM_PREENCHIMENTO_IES && $esdid != WF_EM_AJUSTE_IES) || (date('Ymd') > 20150123) ){
   		 $acao = "
		        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarMantidaExpReg('||me.mneid||');\" title=\"Editar\" >
		    ";
    }

    /*
    $sql = "
        SELECT  '{$acao}' as acao,
                m.mtddsc,
                mundescricao,
                crmdsccurso
        FROM maismedicomec.mantidaexpregulatoria me
        
        JOIN maismedicomec.mantida AS m ON m.mtdid = me.mtdid

        JOIN maismedicomec.mantidamunicipio AS mm ON mm.mtdid = m.mtdid

        left JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
        
        left JOIN maismedicomec.cursomedicina AS cm ON cm.muncod = mm.muncod
        
        JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
        JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}

        WHERE mnestatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}";
    */
    
    $sql = "
        SELECT  '{$acao}' as acao,
                m.mtddsc,
                crmdsccurso,
                mneprogresidencia1, 
                mneprogresidencia2, 
			    mneprogresidencia3, 
			    mneprogresidencia4, 
			    mneprogresidencia5, 
			    mneprogmestdoutorado1, 
			    mneprogmestdoutorado2, 
			    mneprogmestdoutorado3, 
			    mneprogmestdoutorado4, 
			    mneprogmestdoutorado5,
                cs1.casdsccurso as cursosaude1,
                cs2.casdsccurso as cursosaude2,
                cs3.casdsccurso as cursosaude3,
                case when mneaderentefies = 't' then 
                		'SIM'
                	 else
                	 	'NÃO'
                end as fies,
                case when mneaderenteprouni = 't' then 
                		'SIM'
                	 else
                	 	'NÃO'
                end as prouni
        FROM maismedicomec.mantidaexpregulatoria me
        JOIN maismedicomec.mantida AS m ON m.mtdid = me.mtdid
        left JOIN maismedicomec.cursomedicina AS cm ON cm.crmid = me.mnecursomedicina
        left JOIN maismedicomec.cursoareasaude AS cs1 ON cs1.casid = me.mnecursoareasaude1
        left JOIN maismedicomec.cursoareasaude AS cs2 ON cs2.casid = me.mnecursoareasaude2
        left JOIN maismedicomec.cursoareasaude AS cs3 ON cs3.casid = me.mnecursoareasaude3
        WHERE mnestatus = 'A' AND mntid = {$_SESSION['maismedicomec']['mntid']}
        ORDER BY 2";
    	/*
        if(in_array(PERFIL_HOSPITAL, $perfil)){            
            $sql .= " and pe.pflcod = ".PERFIL_HOSPITAL;
            $sql .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
        if(in_array(PERFIL_IES, $perfil)){            
            $sql .= " and pe.pflcod = ".PERFIL_IES;
            $sql .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
		*/
    
    //ver($sql,d);
    $cabecalho = array("Ação", "Mantida", "C.M.", "P.R.1", "P.R.2", "P.R.3", "P.R.4", "P.R.5", "P.M.1", "P.M.2", "P.M.3", "P.M.4", "P.M.5", "C.S.1", "C.S.2", "C.S.3", "FIES", "PROUNI");
    $alinhamento = Array('center');
    $tamanho = Array('4%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
	
?>
