<?php
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
    $perfis = pegaPerfilGeral();
   
    if (!$db->testa_superuser()) {

        if (in_array(PFL_SECRETARIO_ESTADUAL, $perfis) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
            header("location:projovemcampo.php?modulo=principal/listaEstados&acao=A");
            exit;
        }

        if (in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
        	
//         	$sql = "select muncod from projovemcampo.usuarioresponsabilidade where usucpf = '".$_SESSION['usucpf']."' and rpustatus = 'A'";
    
//             $muncod = $db->pegaUm($sql);
        	header("location:projovemcampo.php?modulo=principal/listaMunicipios&acao=A");
            exit;
        }
        if(in_array(PFL_COORDENADOR_TURMA, $perfis) ||in_array(PFL_DIRETOR_ESCOLA, $perfis)){
        	header("location:projovemcampo.php?modulo=principal/listaescolas&acao=A");
        	exit;
        }
    }

    //Chamada de programa

    include APPRAIZ . "includes/cabecalho.inc";

?>

<?php


    echo "<br>";

    $sql = "SELECT count(*) FROM projovemcampo.adesaoprojovemcampo WHERE apcesfera IN('M','C') AND apcstatus='t'";
    $total_mun_part = $db->pegaUm($sql);
     
    $sql = "SELECT count(DISTINCT muncod) FROM projovemcampo.usuarioresponsabilidade WHERE muncod IS NOT NULL AND rpustatus='A' AND pflcod=1182";
    $total_mun_cad = $db->pegaUm($sql);

    $sql = "SELECT count(*) FROM projovemcampo.adesaoprojovemcampo WHERE apcesfera IN('M','C') AND apcstatus='t' and apctermoaceito = 't' ";
    $total_mun_term = $db->pegaUm($sql);

    $sql = "SELECT count(*) FROM projovemcampo.adesaoprojovemcampo WHERE apcesfera IN('E') AND apcstatus='t' --AND ppuid = {$_SESSION['projovemcampo']['ppuid']}";
    $total_est_part = $db->pegaUm($sql);
    
    $sql = "SELECT count(DISTINCT estuf) FROM projovemcampo.usuarioresponsabilidade WHERE estuf IS NOT NULL AND rpustatus='A' AND pflcod=1181";
 	$total_est_cad = $db->pegaUm($sql);
    
    $sql = "SELECT count(*) FROM projovemcampo.adesaoprojovemcampo WHERE apcesfera IN('E')  AND apctermoaceito='t' AND apcstatus='t'-- AND ppuid = {$_SESSION['projovemcampo']['ppuid']}";
	$total_est_term = $db->pegaUm($sql);

	$total_mun_part = ( $total_mun_part  ? $total_mun_part : 0);
	$total_mun_cad = ( $total_mun_cad  ? $total_mun_cad : 0);
	$total_mun_term = ( $total_mun_term  ? $total_mun_term : 0);
	$total_est_part = ( $total_est_part  ? $total_est_part : 0);
	$total_est_cad = ( $total_est_cad  ? $total_est_cad : 0);
	$total_est_term = ( $total_est_term  ? $total_est_term : 0);
	
    echo "<br>";

    if (in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
        $menu = array(0 => array("id" => 1, "descricao" => "Painel", "link" => "/projovemcampo/projovemcampo.php?modulo=inicio&acao=C"),
            1 => array("id" => 2, "descricao" => "Documentos", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/anexoGeral&acao=A")
        );
    } else {
        $menu = array(0 => array("id" => 1, "descricao" => "Documentos", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/anexoGeral&acao=A")
        );
    }

    echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Campo', 'Resumo geral');
    ?>
    <script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
    <script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
    <script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
    <script type="text/javascript">
    
    function exibirRelatorio() {
           
            window.open( 'projovemcampo.php?modulo=relatorio/relatgeral_adesao&acao=A', 'relatoriopersonlizadosprojovem', 'width=1080,height=765,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            
    }
</script>
    <link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

            <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubTituloDireita" width="17%"><b>Municípios participantes:</b></td>
                <td width="17%"><?= $total_mun_part ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemcampo.php?modulo=inicio&acao=C&requisicao=listarMunicipiosCadastrados');"> <b>Municípios secretários cadastrados:</b></td>
                <td width="17%"><?= $total_mun_cad ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemcampo.php?modulo=inicio&acao=C&requisicao=listarMunicipiosCadastrados&adesao=1');"> <b>Municípios adesão aceita:</b></td>
                <td width="17%"><?= $total_mun_term ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="17%"><b>Estados participantes:</b></td>
                <td width="17%"><?= $total_est_part ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemcampo.php?modulo=inicio&acao=C&requisicao=listarEstadosCadastrados');"> <b>Estados secretários cadastrados:</b></td>
                <td width="17%"><?= $total_est_cad ?></td>
                <td class="SubTituloDireita" width="17%"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="displayMessage('projovemcampo.php?modulo=inicio&acao=C&requisicao=listarEstadosCadastrados&adesao=1');"> <b>Estados adesão aceita:</b></td>
                <td width="17%"><?= $total_est_term ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="6"><input type="button" name="filtrar" value="Imprimir Relatório" onclick="exibirRelatorio();"/> </td>
              
            </tr>
        </table>

    <?
    $sql = "SELECT DISTINCT
    				mun.estuf,
                    mun.mundescricao,
    				 (  select metvalor from  projovemcampo.meta met where met.tpmid = 1 and met.apcid = c.apcid) as metvalor,
                    CASE WHEN c.apctermoaceito=TRUE THEN '1' ELSE '' END as adesaosim,
                    CASE WHEN c.apctermoaceito=FALSE THEN '1' ELSE '' END as adesaonao,
	    				CASE WHEN c.apcampliameta=TRUE THEN metas_sugeridas.metvalor::text ELSE '' END as metasugerida,
	                    CASE WHEN c.apcampliameta=TRUE THEN  metas_ajustadas.metvalor::text ELSE '' END as metaajustada,
	                CASE WHEN c.apctermoaceito=TRUE AND c.apctermoajustadoaceito=TRUE 
                             THEN metas_ajustadas.metvalor::varchar(19)
		 				  WHEN c.apctermoaceito=TRUE AND c.apcampliameta=FALSE
                             THEN metasoriginais.metvalor::varchar(19)
                         WHEN c.apctermoaceito=FALSE 
                            THEN metasoriginais.metvalor::varchar(19)
                            ELSE ''
                    END as metafinalizada
    				
                   		
            FROM territorios.municipio as mun
			INNER JOIN territorios.estado as e on e.estuf = mun.estuf
			INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge::character(7) = mun.muncod and apcesfera in ('M') 
    		
    	    LEFT JOIN projovemcampo.meta as metasoriginais on metasoriginais.apcid::varchar(10) = c.apcid::varchar(10) AND metasoriginais.tpmid in ( 1 )
    		LEFT JOIN projovemcampo.meta as metas_sugeridas on metas_sugeridas.apcid::varchar(10) = c.apcid::varchar(10) AND metas_sugeridas.tpmid in (2)
            LEFT JOIN projovemcampo.meta as metas_ajustadas on metas_ajustadas.apcid::varchar(10) =c.apcid::varchar(10) AND metas_ajustadas.tpmid in ( 3 )
           
    		
    		WHERE c.apctermoaceito = 't' AND apcstatus='t'  
			ORDER BY 
				mun.estuf, mun.mundescricao
    ";
// ver($sql,d);
    echo "<table class=listagem width=95% align=center><tr><td class=SubTituloCentro>ADESÃO - Lista de Municípios</td></tr></table>";
    $sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";
    $db->monta_lista_simples($sql_total, array(), 500, 5, 'N', '95%', $par2, 'N', false);

    echo "<div style=\"overflow:auto;height:150;\" >";
    $cabecalho = array("UF", "MUNICÍPIO", "META", "ADESÃO - Sim", "ADESÃO - Não", "Meta - Sugerida", "Meta - Análise", "Adesão Finalizada");
    $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '95%', $par2, 'N', true);
    echo "</div>";

    $sql = "SELECT DISTINCT
			e.estuf,
			e.estdescricao,
			 (  select metvalor from  projovemcampo.meta met where met.tpmid = 1 and met.apcid = c.apcid) as metvalor,
			 CASE WHEN c.apctermoaceito=TRUE THEN '1' ELSE '' END as adesaosim,
                    CASE WHEN c.apctermoaceito=FALSE THEN '1' ELSE '' END as adesaonao,
	    				CASE WHEN c.apcampliameta=TRUE THEN metas_sugeridas.metvalor::text ELSE '' END as metasugerida,
	                    CASE WHEN c.apcampliameta=TRUE THEN  metas_ajustadas.metvalor::text ELSE '' END as metaajustada,
	                 CASE WHEN c.apctermoaceito=TRUE AND c.apctermoajustadoaceito=TRUE 
                             THEN metas_ajustadas.metvalor::varchar(19)
						  WHEN c.apctermoaceito=TRUE AND c.apcampliameta=FALSE
                             THEN metasoriginais.metvalor::varchar(19)
                          WHEN c.apctermoaceito=FALSE 
                            THEN metasoriginais.metvalor::varchar(19)
                            ELSE ''
                    END as metafinalizada
            		
            FROM territorios.estado e 
			inner join projovemcampo.adesaoprojovemcampo c on c.apccodibge::character(2) = e.estcod and apcesfera='E'  
    		 LEFT JOIN projovemcampo.meta as metasoriginais on metasoriginais.apcid::varchar(10) = c.apcid::varchar(10) AND metasoriginais.tpmid in ( 1 )
    		LEFT JOIN projovemcampo.meta as metas_sugeridas on metas_sugeridas.apcid::varchar(10) = c.apcid::varchar(10) AND metas_sugeridas.tpmid in (2)
            LEFT JOIN projovemcampo.meta as metas_ajustadas on metas_ajustadas.apcid::varchar(10) =c.apcid::varchar(10) AND metas_ajustadas.tpmid in ( 3 )
           
	 		WHERE c.apctermoaceito = 't' AND apcstatus='t'
			ORDER BY 
				e.estuf, e.estdescricao";

    echo "<table class=listagem width=95% align=center><tr><td class=SubTituloCentro>ADESÃO - Lista de Estados</td></tr></table>";
    $sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";

    $db->monta_lista_simples($sql_total, array(), 500, 5, 'N', '95%', $par2, 'N', false);

    echo "<div style=\"overflow:auto;height:150;\" >";
    $cabecalho = array("UF", "ESTADO", "META", "ADESÃO - Sim", "ADESÃO - Não", "Meta - Sugerida", "Meta - Análise", "Adesão Finalizada");
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