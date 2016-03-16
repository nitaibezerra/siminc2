<?php
    unset($_SESSION['projovemurbano']['estuf']);
    unset($_SESSION['projovemurbano']['muncod']);
//     ver(d);
    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }

    $texto = "
        <div style=\"font-size:12px\" >
            <br>
                <b>
                    <a target=\"_blank\" title=\"Clique aqui para baixar o arquivo.\" href=\"http://simec.mec.gov.br/nota_tecnica_calendario_2012.pdf\">
                        - Nota Técnica nº 79/2012 - Lançamento de Frequência e Entrega de Trabalhos.<br>
                    </a>
                </b>
        </div>
    ";
    $bloq = '0';
    $perfis = pegaPerfilGeral();
    if($_SESSION['projovemurbano']['ppuid']=='3'&&(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_DIRETOR_POLO, $perfis)||in_array(PFL_DIRETOR_ESCOLA, $perfis)||in_array(PFL_DIRETOR_NUCLEO, $perfis))){
    	$sql = "SELECT
    				rapid
    			FROM
    				projovemurbano.projovemurbano pju
    			INNER JOIN projovemurbano.coordenadorresponsavel coor ON coor.pjuid = pju.pjuid AND coor.ppuid = pju.ppuid
    			WHERE
    				pju.ppuid = {$_SESSION['projovemurbano']['ppuid']}
    			AND corcpf = '{$_SESSION['usucpf']}'";
    	$rapid = $db->pegaUm($sql);
    	if($rapid==''){
    		$bloq = '1';
    	}
    }
    if($bloq == '0'){
	    if (!$db->testa_superuser()) {
	        if (in_array(PFL_DIRETOR_POLO, $perfis)) {
	            header("location:projovemurbano.php?modulo=principal/listaPoloNucleo&acao=P");
	            exit;
	        }
	
	        if (in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
	            header("location:projovemurbano.php?modulo=principal/listaPoloNucleo&acao=N");
	            exit;
	        }
	
	        if (in_array(PFL_SECRETARIO_ESTADUAL, $perfis) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
	            header("location:projovemurbano.php?modulo=principal/listaEstados&acao=A");
	            exit;
	        }
	
	        if (in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
	            header("location:projovemurbano.php?modulo=principal/listaMunicipios&acao=A");
	            exit;
	        }
	    }
    }
    
    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";

    include_once APPRAIZ . 'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
?>

<?php

if ($_SESSION['projovemurbano']['ppuano'] && $_SESSION['projovemurbano']['ppuid']){
	
    if ($_SESSION['projovemurbano']['ppuid'] == 1) {
        popupAlertaGeral($texto, '450px', "250px");
    }

    echo "<br>";

    $sql = "SELECT count(*) FROM projovemurbano.cargameta WHERE cmetipo IN('M','C') AND cmestatus='A' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $total_mun_part = $db->pegaUm($sql);

    $sql = "SELECT count(DISTINCT muncod) FROM projovemurbano.usuarioresponsabilidade WHERE muncod IS NOT NULL AND rpustatus='A' AND pflcod='" . PFL_SECRETARIO_MUNICIPAL . "'";
    $total_mun_cad = $db->pegaUm($sql);

    $sql = "SELECT count(DISTINCT MUNCOD) FROM projovemurbano.projovemurbano WHERE muncod IS NOT NULL AND adesaotermo=TRUE AND pjustatus='A' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $total_mun_term = $db->pegaUm($sql);

    $sql = "SELECT count(*) FROM projovemurbano.cargameta WHERE cmetipo IN('E') AND cmestatus='A' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $total_est_part = $db->pegaUm($sql);

    $sql = "SELECT count(DISTINCT estuf) FROM projovemurbano.usuarioresponsabilidade WHERE estuf IS NOT NULL AND rpustatus='A' AND pflcod='" . PFL_SECRETARIO_ESTADUAL . "'";
    $total_est_cad = $db->pegaUm($sql);

    $sql = "SELECT count(*) FROM projovemurbano.projovemurbano WHERE estuf IS NOT NULL AND adesaotermo=TRUE AND pjustatus='A' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $total_est_term = $db->pegaUm($sql);

    echo "<br>";

    if (in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
        $menu = array(0 => array("id" => 1, "descricao" => "Painel", "link" => "/projovemurbano/projovemurbano.php?modulo=inicio&acao=C"),
            		  1 => array("id" => 2, "descricao" => "Documentos", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/anexoGeral&acao=A")
        );
    } else {
        $menu = array(0 => array("id" => 1, "descricao" => "Documentos", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/anexoGeral&acao=A")
        );
    }

    echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Urbano', 'Resumo geral');
    ?>
    <script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
    <script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
    <script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
    <link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
    <script type="text/javascript">
    jQuery(document).ready(function() {
		<?if($_SESSION['projovemurbano']['ppuid']=='3'&&(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_COORDENADOR_MUNICIPAL, $perfis))&&$rapid==''){ ?>
	        jQuery.ajax({
	            type: "POST",
	            url: window.location.href,
	            data: "&requisicao=trocaRangePeriodo",
	            async: false,
	            success: function(msg){
	
	                jQuery( "#dialog-detalhe" ).html(msg);       
	                jQuery( "#dialog-detalhe" ).dialog({
	                    resizable: false,
	                    height:220,
	                    width:500,
	                    modal: true,
	                    open: function(event, ui) { jQuery(".ui-dialog-titlebar-close").hide(); },
	                    show: { effect: 'drop', direction: "up" },
	                });
	            }
	        });
        <?}?>
    });
	    function exibirRelatorio() {
	           
	            window.open( 'projovemurbano.php?modulo=relatorio/relatgeral_adesao&acao=A', 'relatoriopersonlizadosprojovem', 'width=1080,height=765,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	            
	    }
	</script>
	<style>
	.div_muda{
	align:top;
	}
	</style>
	<div id="dialog-detalhe"></div>
            <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubTituloDireita" width="17%"><b>Municípios participantes:</b></td>
                <td width="17%"><?= $total_mun_part ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemurbano.php?modulo=inicio&acao=C&requisicao=listarMunicipiosCadastrados');"> <b>Municípios secretários cadastrados:</b></td>
                <td width="17%"><?= $total_mun_cad ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemurbano.php?modulo=inicio&acao=C&requisicao=listarMunicipiosCadastrados&adesao=1');"> <b>Municípios adesão aceita:</b></td>
                <td width="17%"><?= $total_mun_term ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="17%"><b>Estados participantes:</b></td>
                <td width="17%"><?= $total_est_part ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemurbano.php?modulo=inicio&acao=C&requisicao=listarEstadosCadastrados');"> <b>Estados secretários cadastrados:</b></td>
                <td width="17%"><?= $total_est_cad ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemurbano.php?modulo=inicio&acao=C&requisicao=listarEstadosCadastrados&adesao=1');"> <b>Estados adesão aceita:</b></td>
                <td width="17%"><?= $total_est_term ?></td>
            </tr>
            <?php if( $_SESSION['projovemurbano']['ppuid'] == 3 ) {?>
            <tr>
                <td class="SubTituloDireita" colspan="6"><input type="button" name="filtrar" value="Imprimir Relatório" onclick="exibirRelatorio();"/> </td>
              
            </tr>
            <?php } ?>
        </table>
    <?
    $sql = "SELECT  DISTINCT
    				mun.estuf,
                    mun.mundescricao,
                    (cgm.geral + cgm.juventude + cgm.prisional),
                    CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
                    CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
                    CASE WHEN sta.suaverdade=TRUE THEN sta.suametasugerida::text ELSE '' END as metasugerida,
                    CASE WHEN sta.suaverdade=TRUE THEN sta.suametaajustada::text ELSE '' END as metaajustada,
                    CASE WHEN sta.suaverdade=TRUE AND pro.adesaotermoajustado=TRUE 
                             THEN metas_ajustadas.mtpvalor::varchar(19)
                      		 WHEN pro.adesaotermo=TRUE
                             THEN (cgm.geral + cgm.juventude + cgm.prisional)::text
                             ELSE ''
                    END as metafinalizada
			
            FROM projovemurbano.projovemurbano pro 
            INNER JOIN territorios.municipio mun ON mun.muncod = pro.muncod 
            INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = mun.muncod and cgm.ppuid = {$_SESSION['projovemurbano']['ppuid']}
            LEFT JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
           	LEFT JOIN projovemurbano.metasdoprograma as metasoriginais on metasoriginais.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metasoriginais.tpmid in ( 7, 13 )AND metasoriginais.mtpvalor != 0 AND metasoriginais.mtpvalor is not NULL
			LEFT JOIN projovemurbano.metasdoprograma as metas_ajustadas on metas_ajustadas.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metas_ajustadas.tpmid in ( 9, 15 ) AND metas_ajustadas.mtpvalor != 0 AND metas_ajustadas.mtpvalor is not NULL
			WHERE pro.adesaotermo IS NOT NULL AND pro.muncod IS NOT NULL
            AND pro.ppuid = {$_SESSION['projovemurbano']['ppuid']}
            AND adesaotermo=TRUE AND pro.pjustatus='A'
            ORDER BY 
				mun.estuf, 
			mun.mundescricao
    ";
// ver($sql);
    echo "<table class=listagem width=95% align=center><tr><td class=SubTituloCentro>ADESÃO - Lista de Municípios</td></tr></table>";
    $sql_total = "SELECT 'Total de registros:', COUNT(adesaosim) FROM ( $sql ) foo";
    $db->monta_lista_simples($sql_total, array(), 500, 5, 'N', '95%', $par2, 'N', false);

    echo "<div style=\"overflow:auto;height:150;\" >";
    $cabecalho = array("UF", "MUNICÍPIO", "META", "ADESÃO - Sim", "ADESÃO - Não", "META - Sugerida", "META - Análise", "Adesão Finalizada");
    $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '95%', $par2, 'N', true);
    echo "</div>";
	
    $sql = "SELECT DISTINCT
			est.estuf,
			est.estdescricao,
			(cgm.geral + cgm.juventude + cgm.prisional),
			CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
			CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
							FROM
								projovemurbano.metasdoprograma mtp
							WHERE
								tpmid in (8,14,11) AND mtp.pjuid = pro.pjuid)::text 
						ELSE cgm.cmemeta::text
				END as metasugerida,
						
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE					
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
				ELSE '' 
			END as metaajustada,
						
			 CASE WHEN sta.suaverdade=TRUE AND pro.adesaotermoajustado=TRUE 
                             THEN (SELECT SUM(mtpvalor)
									FROM
										projovemurbano.metasdoprograma mtp
									WHERE					
										tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
                     		 WHEN pro.adesaotermo=TRUE
                             THEN (cgm.geral + cgm.juventude + cgm.prisional)::text
                             ELSE ''
                    END as metafinalizada
			
			FROM projovemurbano.projovemurbano pro 
			INNER JOIN territorios.estado est ON est.estuf = pro.estuf 
			INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = est.estcod and cgm.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
			LEFT JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
			LEFT JOIN projovemurbano.metasdoprograma as metasoriginais on metasoriginais.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metasoriginais.tpmid in ( 7, 13 )AND metasoriginais.mtpvalor != 0 AND metasoriginais.mtpvalor is not NULL
			LEFT JOIN projovemurbano.metasdoprograma as metas_ajustadas on metas_ajustadas.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metas_ajustadas.tpmid in ( 9, 15 ) AND metas_ajustadas.mtpvalor != 0 AND metas_ajustadas.mtpvalor is not NULL
			
			WHERE 
				pro.adesaotermo IS NOT NULL 
			AND pro.estuf IS NOT NULL
			AND pro.ppuid = {$_SESSION['projovemurbano']['ppuid']}
			AND pjustatus='A'
			ORDER BY 
				est.estuf";
// ver($sql,d);
    echo "<table class=listagem width=95% align=center><tr><td class=SubTituloCentro>ADESÃO - Lista de Estados</td></tr></table>";
    $sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";
    $db->monta_lista_simples($sql_total, array(), 500, 5, 'N', '95%', $par2, 'N', false);

    echo "<div style=\"overflow:auto;height:150;\" >";
    $cabecalho = array("UF", "ESTADO", "META", "ADESÃO - Sim", "ADESÃO - Não", "META - Sugerida", "META - Análise", "Adesão Finalizada");
    $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '95%', $par2, 'N', true);
    echo "</div>";
    ?>

    <script type="text/javascript">

                        messageObj = new DHTML_modalMessage();	// We only create one object of this class
                        messageObj.setShadowOffset(5);	// Large shadow

                        function displayMessage(url) {
                            messageObj.setSource(url);
                            messageObj.setCssClassMessageBox(false);
                            messageObj.setSize(690, 400);
                            messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
                            messageObj.display();
                        }

                        function displayStaticMessage(messageContent, cssClass) {
                            messageObj.setHtmlContent(messageContent);
                            messageObj.setSize(600, 150);
                            messageObj.setCssClassMessageBox(cssClass);
                            messageObj.setSource(false);	// no html source since we want to use a static message here.
                            messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
                            messageObj.display();
                        }

                        function closeMessage() {
                            messageObj.close();
                        }

    </script>
<?} ?>