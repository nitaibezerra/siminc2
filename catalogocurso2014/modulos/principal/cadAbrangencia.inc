<?php
global $db;

$curid = $_SESSION['catalogo']['curid'];

if(!$curid){
    echo "<script>
			window.location = 'catalogocurso2014.php?modulo=inicio&acao=C';
		  </script>";
    exit();
}

if(isset($_POST['action']) && !empty($_POST['action'])){
    $action = $_POST['action'];
} else {
    $action = '';
}

switch($action):
    case 'salvar':
        $sql = "SELECT 	abcid
                FROM 	catalogocurso2014.abrangenciacurso
                WHERE 	curid = {$curid}";
        
        $idDb = $db->pegaUm($sql);

        $abcopcao = $_POST['abcopcao'];
        $abcprematricula = $_POST['abcprematricula'];
        $abcdemanda = $_POST['abcdemanda'];
        $abcexibirsisfor = $_POST['abcexibirsisfor'];

        if($abcprematricula == 'false'){
            $abcopcao = 1;
        }
        
        if($idDb){
            $sql = "UPDATE 		catalogocurso2014.abrangenciacurso
                    SET 		curid = {$curid}, 
                    			abcopcao = {$abcopcao}, 
                    			abcprematricula = {$abcprematricula}, 
                    			abcdemanda = {$abcdemanda}, 
                    			abcexibirsisfor = {$abcexibirsisfor}, 
                    			abcstatus='A'
                    WHERE 		abcid = {$idDb}
                    RETURNING 	abcid";
        } else {
            $sql = "INSERT INTO catalogocurso2014.abrangenciacurso(curid, abcopcao, abcprematricula, abcdemanda, abcexibirsisfor, abcstatus)
                    VALUES ({$curid}, {$abcopcao}, {$abcprematricula}, {$abcdemanda}, {$abcexibirsisfor}, 'A')
                    RETURNING abcid";
        }
        
        $id = $db->pegaUm($sql);

        if($id){
            $sql = "DELETE FROM catalogocurso2014.interstadual WHERE abcid = {$id};";
            $sql .= "DELETE FROM catalogocurso2014.abrangenciageral WHERE abcid = {$id};";
            $db->executar($sql);

            if($abcopcao == 2){
                $sql = '';
                if($_POST['estuf'] && is_array($_POST['estuf']) && count($_POST['estuf']) > 0){
                    foreach($_POST['estuf'] as $estuf){
                        $sql .= "INSERT INTO catalogocurso2014.interstadual (estuf , abcid) VALUES ('{$estuf}' , {$id}); ";
                    }
                }
                $db->executar($sql);
            } else if($abcopcao == 3){
                $sql = '';
                if($_POST['abgid'] && is_array($_POST['abgid']) && count($_POST['abgid']) > 0){
                    foreach($_POST['abgid'] as $estuf){
                        $estuf = explode('-' , $estuf);
                        $estuf = $estuf[0];
                        $sql .= "INSERT INTO catalogocurso2014.abrangenciageral (estuf , abcid) VALUES ('{$estuf}' , {$id}); ";
                    }
                }
                $db->executar($sql);
            }
            echo 'Salvo com sucesso!';
            $db->commit();
        } else {
            echo 'Erro ao salvar!';
        }
        exit();
        break;
    default:
        //Chamada de programa
        include  APPRAIZ."includes/cabecalho.inc";
        if($_SESSION['catalogo']['curid']){
            monta_abas();
        } else {
            monta_abas(57394);
        }

        if( $_SESSION['catalogo']['curid'] ){
            $permissoes = testaPermissao();
        } else {
            $permissoes['gravar'] = true;
        }

        $sql = "SELECT 	*
                FROM 	catalogocurso2014.abrangenciacurso
                WHERE 	curid = {$curid}";
        
        $abrangencia = $db->pegaLinha($sql);

        if($abrangencia){
            if($abrangencia['abcopcao'] == 2){
                $sql = "SELECT 		est.estuf AS codigo, 
                					est.estdescricao AS descricao
                        FROM 		catalogocurso2014.interstadual intf
                        LEFT JOIN 	territorios.estado est ON (est.estuf = intf.estuf)
                        WHERE 		abcid = {$abrangencia['abcid']}
                        ORDER BY 	est.estdescricao;";
                $estuf = $db->carregar($sql);
                $abgid = false;

            } else if($abrangencia['abcopcao'] == 3) {
                $sql = "SELECT 		DISTINCT estuf || '-UF' AS codigo,
                                    (estuf) as descricao
                        FROM        territorios.estado
                        WHERE       estuf NOT IN (
                                    -- uf dos cursos cadastrados na ies ofertante
                                    SELECT 		DISTINCT (enc.estuf || '-UF') AS codigo
                                    FROM 		catalogocurso2014.iesofertante ieo
                                    INNER JOIN  catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
                                    INNER JOIN  catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
                                    INNER JOIN  entidade.entidade ent ON ent.entunicod = ieo.unicod
                                    INNER JOIN  entidade.endereco enc ON enc.entid = ent.entid
                                    WHERE  		ieo.ieostatus = 'A'
                                                AND (coo.coordsigla ILIKE '%SECADI%'OR coo.coordsigla ILIKE '%SEB%')
                                                -- CÓDIGO DO CURSO
                                                AND cur.curid = {$curid} AND ieoanoprojeto = {$_SESSION['exercicio']}
                                    UNION ALL
                                    -- situacao do curso no sisfor
                                    SELECT 		DISTINCT enc.estuf || '-UF' AS codigo
                                    FROM 		sisfor.sisfor sif
                                    LEFT JOIN 	sisfor.sisfories sies ON sies.unicod = sif.unicod
                                  	INNER JOIN 	catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid AND ieo.ieostatus = 'A'
                                    INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
                                    INNER JOIN 	entidade.entidade ent ON ent.entunicod = ieo.unicod
                                 	INNER JOIN 	entidade.endereco enc ON enc.entid = ent.entid
                                    LEFT JOIN 	workflow.documento doc ON doc.docid = sies.docid
                                    LEFT JOIN 	workflow.estadodocumento est ON est.esdid = doc.esdid
                                    WHERE		sif.sifstatus = 'A' AND cur.curid = {$curid} AND ieoanoprojeto = {$_SESSION['exercicio']})
                                                AND estuf IN (SELECT 		est.estuf
                                                      		  FROM 			catalogocurso2014.abrangenciageral intf
                                                      		  LEFT JOIN 	territorios.estado est ON (est.estuf = intf.estuf)
                                                      		  WHERE 		abcid = {$abrangencia['abcid']}
                                                      		  ORDER BY 		est.estdescricao )
                                    UNION ALL
                                    -- curso do cadastrado na IES Ofertante
                                    SELECT  	DISTINCT UPPER((enc.estuf || '-' || UPPER (ent.entnome) )) AS codigo,
                                                ( enc.estuf || ' - ' || ieo.unicod || ' - ' || UPPER (ent.entnome)) AS descricao
                                    FROM 		catalogocurso2014.iesofertante ieo
                                    INNER JOIN  catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
                                    INNER JOIN  catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
                                    INNER JOIN  entidade.entidade ent ON ent.entunicod = ieo.unicod
                                    INNER JOIN  entidade.endereco enc ON enc.entid = ent.entid
                                    WHERE  		ieo.ieostatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%'OR coo.coordsigla ILIKE '%SEB%')
                                                AND cur.curid NOT IN (SELECT 		DISTINCT cur.curid
                                                                      FROM 			sisfor.sisfor sif
                                                                      LEFT JOIN 	sisfor.sisfories sies ON sies.unicod = sif.unicod
                                                                      INNER JOIN 	catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid AND ieo.ieostatus = 'A'
                                                                      INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid
                                                                                     -- CÓDIGO DO CURSO
                                                                      WHERE 		cur.curid = {$curid})
                                            	-- CÓDIGO DO CURSO
                                            	AND cur.curid = {$curid}  AND ieoanoprojeto = {$_SESSION['exercicio']}
                                            	AND enc.estuf IN (SELECT 			est.estuf
                                                      			  FROM 				catalogocurso2014.abrangenciageral intf
                                                      			  LEFT JOIN 		territorios.estado est ON (est.estuf = intf.estuf)
                                                      			  WHERE 			abcid = {$abrangencia['abcid']}
                                                      			  ORDER BY 			est.estdescricao )
                                    UNION ALL
                                    -- situacao do curso no sisfor
                                    SELECT 		DISTINCT UPPER((enc.estuf || '-' || UPPER (ent.entnome) )) AS codigo,
                                                (UPPER (enc.estuf || ' - ' || ieo.unicod  || ' - ' || ent.entnome)  || ' - ' ||
                                                CASE WHEN sif.sifopcao = '1' THEN 'Aceito'
                                                	 WHEN sif.sifopcao = '2' THEN 'Rejeitado'
                                                	 WHEN sif.sifopcao = '3' THEN 'Repactuado' END  || ' - ' ||
                                                CASE WHEN est.esdid = '1082' THEN 'Finalizado' ELSE est.esddsc END) AS descricao
                                    FROM 		sisfor.sisfor sif
                                    LEFT JOIN 	sisfor.sisfories sies ON sies.unicod = sif.unicod
                                    INNER JOIN 	catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid AND ieo.ieostatus = 'A'
                                    INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
                                    INNER JOIN 	entidade.entidade ent ON ent.entunicod = ieo.unicod
							        INNER JOIN 	entidade.endereco enc ON enc.entid = ent.entid
                                    LEFT JOIN 	workflow.documento doc ON doc.docid = sies.docid
                                    LEFT JOIN 	workflow.estadodocumento est ON est.esdid = doc.esdid
                                    WHERE		sif.sifstatus = 'A' AND
                                                -- CÓDIGO DO CURSO
                                                cur.curid = {$curid}
												AND enc.estuf IN (SELECT 		est.estuf
                                                      			  FROM 			catalogocurso2014.abrangenciageral intf
                                                      			  LEFT JOIN 	territorios.estado est ON (est.estuf = intf.estuf)
                                                      			  WHERE 		abcid = {$abrangencia['abcid']}
                                                      			  ORDER BY 		est.estdescricao )
                                    ORDER BY 	1";
                $abgid = $db->carregar($sql);
                $estuf = false;

            } else {
                $abgid = false;
                $estuf = false;
            }
        } else {
            $abgid = false;
            $estuf = false;
        } ?>
            <div class="row">
                <div class="col-md-12">
                    <ul class="breadcrumb">
                        <li>
                            <strong>Curso: </strong>
                            <?php echo $_SESSION['catalogo']['curid'] .' - ' . $_SESSION['catalogo']['curdesc']; ?>
                        </li>
                    </ul>
                </div>
            </div>
            <form id="form_abrangencia">
                <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
                    <tr>
                        <td class="SubTituloDireita" width="15%">
                            Capturar Pré-matrícula via PDDE Interativo?
                        </td>
                        <td>
                            <input name="abcprematricula" type="radio" required="required" value="true" <?php if(($abrangencia['abcprematricula'] == 't')) echo 'checked="checked"'; ?>/> Sim
                            <input name="abcprematricula" type="radio" required="required" value="false" <?php if(($abrangencia['abcprematricula'] == 'f')) echo 'checked="checked"'; ?>/> Não
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                    <tr class="opcao" <?php if($abrangencia['abcprematricula'] == 'f') echo 'style="display:none;"'; ?>>
                        <td class="SubTituloDireita" width="15%">Opção</td>
                        <td>
                            <input name="abcopcao" type="radio" required="required" value="1" <?php if($abrangencia['abcopcao'] == '1') echo 'checked="checked"'; ?> /> Nacional
                            <input name="abcopcao" type="radio" required="required" value="2" <?php if($abrangencia['abcopcao'] == '2') echo 'checked="checked"'; ?> /> Interestadual
                            <input name="abcopcao" type="radio" required="required" value="3" <?php if($abrangencia['abcopcao'] == '3') echo 'checked="checked"'; ?> /> IES Ofertante
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                    <tr class="estado_ies" <?php if($abrangencia['abcopcao'] != '3') echo 'style="display: none;" '; ?>>
                        <td class="SubTituloDireita" width="15%">Estados da IES Ofertante</td>
                        <td>
                        <?php

                            $sql = "SELECT 			DISTINCT estuf || '-UF' AS codigo,
                                        			(estuf) AS descricao
                                    FROM            territorios.estado
                                    WHERE           estuf NOT IN (
                                    				-- uf dos cursos cadastrados na ies ofertante
				                                    SELECT 		DISTINCT  enc.estuf || '-UF' AS codigo
				                                    FROM 		catalogocurso2014.iesofertante ieo
				                                    INNER JOIN  catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
				                                    INNER JOIN  catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
				                                    INNER JOIN  entidade.entidade ent ON ent.entunicod = ieo.unicod
				                                    INNER JOIN  entidade.endereco enc ON enc.entid = ent.entid
				                                    WHERE  		ieo.ieostatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%'OR coo.coordsigla ILIKE '%SEB%')
				                                                AND cur.curid = {$curid} AND ieoanoprojeto = {$_SESSION['exercicio']}
                            				        UNION ALL
                                    				-- situacao do curso no sisfor
                                    				SELECT 		DISTINCT enc.estuf || '-UF' AS codigo
                                                    FROM 		sisfor.sisfor sif
                                                    LEFT JOIN 	sisfor.sisfories sies ON sies.unicod = sif.unicod
                                                    INNER JOIN 	catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid AND ieo.ieostatus = 'A'
                                                    INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid           AND cur.curstatus = 'A'
                                                    INNER JOIN 	entidade.entidade ent ON ent.entunicod = ieo.unicod
                                                    INNER JOIN 	entidade.endereco enc ON enc.entid = ent.entid
                                                    LEFT JOIN 	workflow.documento doc ON doc.docid = sies.docid
                                                    LEFT JOIN 	workflow.estadodocumento est ON est.esdid = doc.esdid
                                                    WHERE       sif.sifstatus = 'A' AND cur.curid = {$curid}) AND ieoanoprojeto = {$_SESSION['exercicio']}
                                    				UNION ALL
				                                    -- curso do cadastrado na IES Ofertante
                				                    SELECT  	DISTINCT UPPER((enc.estuf || '-' || UPPER (ent.entnome) )) AS codigo,
                                                    			( enc.estuf || ' - ' || ieo.unicod || ' - ' || UPPER (ent.entnome)) AS descricao
                                            		FROM 		catalogocurso2014.iesofertante ieo
                                      			    INNER JOIN  catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
                                            		INNER JOIN  catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
                                            		INNER JOIN  entidade.entidade ent ON ent.entunicod = ieo.unicod
                                            		INNER JOIN  entidade.endereco enc ON enc.entid = ent.entid
                                            		WHERE  		ieo.ieostatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%'OR coo.coordsigla ILIKE '%SEB%')
                                                    			AND cur.curid NOT IN (
                                                    								 	SELECT 		DISTINCT cur.curid
                                                                                        FROM 		sisfor.sisfor sif
                                                                                        LEFT JOIN 	sisfor.sisfories sies ON sies.unicod = sif.unicod
                                                                                        INNER JOIN 	catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid AND ieo.ieostatus = 'A'
                                                                                        INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid
                                                                                        			-- CÓDIGO DO CURSO
                                                                                        WHERE 		cur.curid = {$curid})
                                            					AND cur.curid = {$curid} AND ieoanoprojeto = {$_SESSION['exercicio']}
	                                  				UNION ALL
				                                    -- situacao do curso no sisfor
                                    				SELECT 		DISTINCT UPPER((enc.estuf || '-' || UPPER (ent.entnome) )) AS codigo,
                                                    			(enc.estuf || ' - ' || ieo.unicod  || ' - ' || UPPER (ent.entnome)  || ' - ' ||
                                                				CASE WHEN sif.sifopcao = '1' THEN 'Aceito'
                                                					 WHEN sif.sifopcao = '2' THEN 'Rejeitado'
                                                					 WHEN sif.sifopcao = '3' THEN 'Repactuado' END  || ' - ' ||
                                                				CASE WHEN est.esdid = '1082' THEN 'Finalizado' ELSE est.esddsc END) AS descricao
                                                    FROM 		sisfor.sisfor sif
                                                    LEFT JOIN 	sisfor.sisfories sies ON sies.unicod = sif.unicod
                                                    INNER JOIN 	catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid AND ieo.ieostatus = 'A'
                                                    INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
                                                    INNER JOIN 	entidade.entidade ent ON ent.entunicod = ieo.unicod
                                            		INNER JOIN 	entidade.endereco enc ON enc.entid = ent.entid
                                                    LEFT JOIN 	workflow.documento doc ON doc.docid = sies.docid
                                                    LEFT JOIN 	workflow.estadodocumento est ON est.esdid = doc.esdid
                                                    WHERE       sif.sifstatus = 'A' AND	cur.curid = {$curid} AND ieoanoprojeto = {$_SESSION['exercicio']}
                                                    ORDER BY 	1";
                            
                            combo_popup('abgid', $sql, 'Todos', '400x400', '',
                                $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
                                $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
                                $param_conexao = false, $where=null, $value = $abgid, $mostraPesquisa = true, $campo_busca_descricao = false,
                                $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = array('cod'));
                            ?>
                        </td>
                    </tr>
                    <tr class="estado" <?php if($abrangencia['abcopcao'] != '2') echo 'style="display: none;" '; ?>>
                        <td class="SubTituloDireita" width="15%">Estados</td>
                        <td>
                            <?php
                            $sql = "SELECT 		estuf AS codigo,  
                            					estdescricao AS descricao
                                    FROM 		territorios.estado
                                    ORDER BY 	estdescricao";

                            combo_popup('estuf', $sql, 'Todos', '400x400', '',
                                $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
                                $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
                                $param_conexao = false, $where=null, $value = $estuf, $mostraPesquisa = true, $campo_busca_descricao = false,
                                $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="15%">Capturar Demanda 2015 via PDDE Interativo?</td>
                        <td>
                            <input name="abcdemanda" type="radio" value="true" required="required" <?php if(($abrangencia['abcdemanda'] == 't')) echo 'checked="checked"'; ?>/> Sim
                            <input name="abcdemanda" type="radio" value="false" required="required" <?php if(($abrangencia['abcdemanda'] == 'f')) echo 'checked="checked"'; ?>/> Não
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="15%">
                            Exibir no Planejamento 2014 no SisFor?
                        </td>
                        <td>
                            <input name="abcexibirsisfor" type="radio" value="true" required="required" <?php if(($abrangencia['abcexibirsisfor'] == 't')) echo 'checked="checked"'; ?>/> Sim
                            <input name="abcexibirsisfor" type="radio" value="false" required="required" <?php if(($abrangencia['abcexibirsisfor'] == 'f')) echo 'checked="checked"'; ?>/> Não
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                </table>
            </form>
            <script>
                var jq = jQuery.noConflict();
                jq('.chosen-select').chosen({allow_single_deselect:true});
            </script>
            <script src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js" type="text/javascript"></script>
            <script src="/includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.min.js" type="text/javascript"></script>
            <script src="/includes/JQuery/jquery-1.9.1/funcoes.js" type="text/javascript"></script>
            <link href="/includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.min.css" rel="stylesheet">
            <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
                <tr>
                    <td colspan="2" style="text-align: center;">
                        <input type="button" id="bt_voltar"  value="Voltar"/>
                        <input type="button" id="bt_salvar" value="Salvar" />
                        <input type="button" id="bt_salvarC" value="Salvar e Continuar"/>
                        <input type="button" id="bt_proximo" value="Próximo"/>
                    </td>
                </tr>
            </table>
            <div id="dialog"></div>
            <script type="text/javascript">
                $('input[name=abcprematricula]').click(function(){
                    if($(this).val() == 'true'){
                        $('tr.opcao').fadeIn();
                    } else {
                        $($('input[name=abcopcao]')[0]).click();
                        $('tr.opcao').hide();
                        $('tr.estado_ies').hide();
                    }
                });

                $('input[name=abcopcao]').click(function(){
                    if($(this).val() == '2'){
                        $('tr.estado').fadeIn();
                        $('tr.estado_ies').hide();
                    } else if($(this).val() == '3'){
                        $('tr.estado_ies').fadeIn();
                        $('tr.estado').hide();
                    } else {
                        $('tr.estado').fadeOut();
                    }
                });


                $('#bt_voltar').click(function(){
                    window.location.href = 'catalogocurso2014.php?modulo=principal/cadContato&acao=A';
                });

                $('#bt_proximo').click(function(){
                    window.location.href = 'catalogocurso2014.php?modulo=principal/visualizacao&acao=A';
                });

                $('#bt_salvar').click(function(){
                    salvar();
                });

                $('#bt_salvarC').click(function(){
                    salvar();
                    setTimeout(function(){
                        window.location.href = 'catalogocurso2014.php?modulo=principal/visualizacao&acao=A';
                    } , 1500);
                });

                function salvar(){
                    isValid = true;
                    $('#form_abrangencia').find('input[required]').each(function(){
                        inputCheck = $('input[name=' + $(this).attr('name') + ']');
                        if(!inputCheck.is(':checked')){
                            isValid = false;
                            alert('Campo obrigatório!');
                            inputCheck[0].focus();
                            return false;
                        }
                    });


                    if($('input[name=abcopcao]:checked').val() == 2){
                        selectAllOptions( document.getElementById('estuf') );

                        if(!$('#estuf').val()[0]){
                            alert('Campo obrigatório!');
                            $('#estuf').focus();
                            return false;
                        }
                    } else if($('input[name=abcopcao]:checked').val() == 3){
                        selectAllOptions( document.getElementById('abgid') );

                        if(!$('#abgid').val()[0]){
                            alert('Campo obrigatório!');
                            $('#abgid').focus();
                            return false;
                        }
                    }

                    if(!isValid){
                        return false;
                    }

                    data = jQuery('#form_abrangencia').serialize() + '&action=salvar';

                    jQuery.post(window.location.href, data, function(html){
                        if(html != ''){
                            alert(html);
                        }
                    });
                }

                <?php if(!$permissoes['gravar']): ?>
                setTimeout(function(){
                    $('select , textarea , input').attr('disabled' , 'disabled');
                    $('input:button').removeAttr('disabled' , 'disabled');
                    $('#bt_salvar , #bt_salvarC').attr('disabled' , 'disabled');

                } , 500);
                <?php endif ?>

            	$(document).ready(function() {
            		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
            	    jQuery('#top_content_label_sistema').html(exercicio);
            	});                
            </script>
        <?php
        break;
endswitch;
?>