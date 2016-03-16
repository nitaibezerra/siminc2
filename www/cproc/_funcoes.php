<?php

/**
 * Funçãoq ue verifica existência de prcid
 *
 * @author Sávio Resende
 */
function verificaExistenciaPrcid(){
    if( !$_SESSION['cproc']['prcid'] )
        return false;
    else
        return true;
}

function pegarPerfil($usucpf){
	global $db;

	$sql = "SELECT          pu.pflcod
            FROM            seguranca.perfilusuario pu
            INNER JOIN      seguranca.perfil p ON p.pflcod = pu.pflcod
            AND             pu.usucpf = '{$usucpf}'
            AND             p.sisid = {$_SESSION['sisid']}
            AND             pflstatus = 'A'";

	$arrPflcod = $db->carregar($sql);
	!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
	$arrPerfil = array();
	foreach($arrPflcod as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	return $arrPerfil;
}

function salvarProcesso($post){
    global $db;

    extract($post);

    $mntid              = $mntid[0] ? $mntid[0] : "NULL";
    $iesid              = $iesid[0] ? $iesid[0] : "NULL";
    $entid              = $entid[0] ? $entid[0] : "NULL";
    $muncod             = $muncod ? "'{$muncod}'" : "NULL";
    $estuf              = $estuf ? "'{$estuf}'" : "NULL";
    $prcdtentradaproc   = formata_data_sql($prcdtentradaproc);
    $prccurso           = $prccurso ? "'{$prccurso}'" : "NULL";
    $prclocaloferta     = $prclocaloferta ? "'{$prclocaloferta}'" : "NULL";
    $prcassunto         = $prcassunto ? "'{$prcassunto}'" : "NULL";
    $prcstatusead       = $prcstatusead == 'N' ? 'FALSE' : 'TRUE';
        
    $sql = "
        INSERT INTO cproc.processo(
                mntid, 
                iesid, 
                entid,
                muncod, 
                estuf, 
                tprid, 
                prccurso, 
                prclocaloferta, 
                prcnumsidoc,
                prcdtentradaproc, 
                prcstatusead, 
                prcassunto, 
                prcdtinclusao, 
                prcstatus, 
                prcstatussupesp
            )VALUES(
                {$mntid}, 
                {$iesid}, 
                {$entid},
                {$muncod}, 
                {$estuf}, 
                {$tprid}, 
                {$prccurso}, 
                {$prclocaloferta}, 
                '{$prcnumsidoc}', 
                '{$prcdtentradaproc}', 
                {$prcstatusead}, 
                {$prcassunto}, 
                NOW(), 
                'A', 
                '{$prcstatussupesp}'
        ) RETURNING prcid
    ";
                //ver($sql, d);
    // exit($sql);
    $prcid = $db->pegaUm($sql);    
        
    if($prcid){
    	
    	$docdsc = "Fluxo do processo - ID " . $prcid;
    	// cria documento
    	$docid = wf_cadastrarDocumento(CPROC_GESTAODOCUMENTOSDISUP_TPDID, $docdsc );
    	$sql = "UPDATE cproc.processo SET docid='".$docid."' WHERE prcid='".$prcid."'";
    	$db->executar($sql);
    	
        $db->commit();
        $parametros = "&prcid=".$prcid;
        $db->sucesso('principal/cad_dados_processo', $parametros, 'Processo cadastrado com sucesso!', 'N', 'N');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', 'principal/dadosprocesso&acao=A');
    }
}

function alterarProcesso($post){
    global $db;

    extract($post);
    
    // Retorna a Mantida e a Mantenedora atual do processo
    $sqlH = "select mntid, iesid from cproc.processo where prcid = {$prcid}";
    
    $resH = $db->pegaLinha($sqlH);
    
    $mntid              = $mntid[0] ? $mntid[0] : "NULL";
    $iesid              = $iesid[0] ? $iesid[0] : "NULL";
    $entid              = $entid[0] ? $entid[0] : "NULL";
    $prcdtentradaproc   = formata_data_sql($prcdtentradaproc);
    $prccurso           = $prccurso ? "'{$prccurso}'" : "null";
    $prclocaloferta     = $prclocaloferta ? "'{$prclocaloferta}'" : "null";
    //$entid       = $entid ? "'{$entid}'" : "null";
    $prcassunto         = $prcassunto ? "'{$prcassunto}'" : "null";
    $prcstatusead       = $prcstatusead == 'N' ? 'FALSE' : 'TRUE';
    

    $sql = "
        UPDATE cproc.processo
            SET mntid            = {$mntid},
                iesid            = {$iesid},
                muncod           = '{$muncod}',
                entid      		 = {$entid},
                estuf            = '{$estuf}',
                tprid            = {$tprid},
                prccurso         = {$prccurso},
                prclocaloferta   = {$prclocaloferta},
                prcnumsidoc      = '{$prcnumsidoc}',
                prcstatussupesp  = '{$prcstatussupesp}',
                prcdtentradaproc = '{$prcdtentradaproc}',
                prcstatusead     = {$prcstatusead},
                prcassunto       = {$prcassunto}
        WHERE prcid = {$prcid} RETURNING prcid;
    ";
	
    $prcid = $db->pegaUm($sql);
        
    if($prcid){
    	// Verifica se houve alteração na Mantida ou Mantenedora
        if ($resH['mntid'] != $mntid || $resH['iesid'] != $iesid) {
        	// Se houver alteração, grava um registro na tabela de histórico
    		$sqlH = "INSERT INTO cproc.historico (prcid, mntid, iesid, hstdataalteracao, usucpf) VALUES ({$prcid}, {$resH['mntid']}, {$resH['iesid']}, NOW(), '{$_SESSION['usucpf']}')";
    		$hstid = $db->pegaUm($sqlH);
    	}
    	$db->commit();
        $parametros = "&prcid=".$prcid;
        $db->sucesso('principal/cad_dados_processo', $parametros, 'Processo alterado com sucesso!', 'N', 'N');
    } else {
            $db->insucesso('Não foi possível realizar a operação.', '', 'principal/cad_dados_processo&acao=A');
    }
}

function exibirProcesso($prcid){
	global $db;

	if($prcid){
		$aryWhere[] = "prcid = {$prcid}";
	}

	$sql = "SELECT 		prcid,
						mntid,
						iesid,
						muncod,
						estuf,
						tprid,
						prccurso,
                        entid,
       					prclocaloferta,
                        prcstatussupesp,
       					prcnumsidoc,
       					prcdtentradaproc,
       					prcstatusead,
       					prcassunto,
       					prcdtinclusao,
       					prcstatus
		 	FROM 		cproc.processo
		 				".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";
	$processo = $db->pegaLinha($sql);
	return $processo;
}


function carregarMunicipiosPorUF($post) {
    global $db;

    extract($post);

    $sql = "
        SELECT  muncod AS codigo,
                mundescricao AS descricao
        FROM territorios.municipio
        
        WHERE estuf = '{$estuf}'
            
        ORDER BY mundescricao
    ";
    $db->monta_combo('muncod', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', '', '','','Município');
}

function carregarMantenedoraPorMantida($iesid) {
	global $db;
	
	$sql = "SELECT mnt.mntid, mnt.mntdsc, ies.iesid FROM gestaodocumentos.mantenedoras mnt INNER JOIN gestaodocumentos.instituicaoensino ies ON mnt.mntid = ies.mntid WHERE iesid = '{$iesid}'";
	
	$res = $db->carregar($sql);
	
	echo $res[0]['mntid']."|".$res[0]['mntdsc'];
}

function listarProcesso( $dados = null, $tipo = 'html' ) {
    global $db;

    $perfil = pegarPerfil( $_SESSION['usucpf'] );

    if( $dados ) {
        extract($dados);
    }

    if( $dados['prcid'] != "" ){
        $prcid = $dados['prcid'];
        $arrWhere[] = " prc.prcid = '{$prcid}'";
    }
    if( $dados['tprid'] != "" ){
        $tprid = $dados['tprid'];
        $arrWhere[] = " tpr.tprid = '{$tprid}'";
    }
    if( $dados['prcnumsidoc'] != "" ){
        $prcnumsidoc = $dados['prcnumsidoc'];
        $arrWhere[] = " prc.prcnumsidoc = '{$prcnumsidoc}'";
    }
    if( $dados['prccurso'] != "" ){
        $prccurso = $dados['prccurso'];
        $arrWhere[] = " public.removeacento(prc.prccurso) ILIKE public.removeacento( ('%{$prccurso}%') ) ";
    }
    if( $dados['modid'] != "" ){
        $modid = $dados['modid'];
        $arrWhere[] = " prc.modid = '{$modid}'";
    }
    if( $dados['prclocaloferta'] != "" ){
        $prclocaloferta = $dados['prclocaloferta'];
        $arrWhere[] = " public.removeacento(prc.prclocaloferta) ILIKE public.removeacento( ('%{$prclocaloferta}%') ) ";
    }
    if( $dados['prcpoloferta'] != "" ){
        $prcpoloferta = $dados['prcpoloferta'];
        $arrWhere[] = " public.removeacento(prc.prcpoloferta) ILIKE public.removeacento( ('%{$prcpoloferta}%') ) ";
    }
    if( $dados['estuf'] != "" ){
        $estuf = $dados['estuf'];
        $arrWhere[] = " mun.estuf = '{$estuf}'";
    }
    if( $dados['muncod'] != "" ){
        $muncod = $dados['muncod'];
        $arrWhere[] = " mun.muncod = '{$muncod}'";
    }
    if( $dados['entid'] != "" ){
    	$entid = $dados['entid'];
    	$arrWhere[] = " prc.entid = '{$entid}'";
    }
    if( $dados['coocpf'] != "" ){
    	$coocpf = $dados['coocpf'];
    	$arrWhere[] = " cr.usucpf = '{$coocpf}'";
    }
    if( $dados['teccpf'] != "" ){
    	$teccpf = $dados['teccpf'];
    	$arrWhere[] = " tr.usucpf = '{$teccpf}'";
    }
    if( $dados['fasid'] != "" ){
        $fasid = $dados['fasid'];
        $arrWhere[] = " prc.prcid in (select p.prcid
                                      FROM cproc.processo p
                                      inner join (select max(fprid) as fprid, prcid from cproc.faseprocesso group by prcid) f on f.prcid = p.prcid
                                      inner join cproc.faseprocesso f2 on f2.fprid = f.fprid
                                      where f2.fasid={$fasid})";
    }
    if( $dados['stsid'] != "" ){
        $stsid = $dados['stsid'];
        $arrWhere[] = " prc.prcid in (select p.prcid
                                      FROM cproc.processo p
                                      inner join (select max(stpid) as stpid, prcid from cproc.statusprocesso group by prcid) s on s.prcid = p.prcid
                                      inner join cproc.statusprocesso s2 on s2.stpid = s.stpid
                                      where s2.stsid={$stsid})";
    }

    if( $dados['pmes'] != "" && $dados['pano'] != "" ){
        $pmes = $dados['pmes'];
        $pano = $dados['pano'];
        if(strlen($pmes) == 1) $pmes = "0".$pmes;

        $arrWhere[] = "esd.esdid in (1312)";
        $arrWhere[] = "( (SELECT to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:SS') FROM workflow.historicodocumento WHERE docid=doc.docid) between '{$pano}-{$pmes}-01 00:00:00' and '{$pano}-{$pmes}-31 23:59:59')";
    }

    
    /*if(in_array(PERFIL_APOIO_CPROC, $perfil)){
    	//$arrWhere[] = "esd.esdid in (1007,1004,1006,1000,1005)";
    	$arrWhere[] = "esd.esdid in (1311,1305,1306,1313,1309)";
	}
    if(in_array(PERFIL_COORDENADOR_CPROC, $perfil)){
    	//$arrWhere[] = "esd.esdid in (1008,1001)";
    	$arrWhere[] = "esd.esdid in (1308,1307,1310)";
    	$arrWhere[] = "cr.usucpf = '{$_SESSION['usucpf']}'";
    }
    if(in_array(PERFIL_TECNICO_CPROC, $perfil)){
    	//$arrWhere[] = "esd.esdid in (1002)";
    	$arrWhere[] = "esd.esdid in (1312)";
    	$arrWhere[] = "tr.usucpf = '{$_SESSION['usucpf']}'";
    }*/
    
	
     if( $dados['processo_administrativo'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.processoadmin pa where prc.prcid = pa.prcid) > 0";
    }elseif($dados['processo_administrativo'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.processoadmin pa where prc.prcid = pa.prcid) = 0";
    }
    
     if( $dados['medida_cautelar'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.medidacautelar mc where prc.prcid = mc.prcid) > 0";
    }elseif($dados['medida_cautelar'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.medidacautelar mc where prc.prcid = mc.prcid) = 0";
    }
    
     if( $dados['medida_saneadora'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.medidasaneadora ms where prc.prcid = ms.prcid) > 0";
    }elseif($dados['medida_saneadora'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.medidasaneadora ms where prc.prcid = ms.prcid) = 0";
    }
    
     if( $dados['penalidade_aplicada'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.penalidadeaplicada pea where prc.prcid = pea.prcid) > 0";
    }elseif($dados['penalidade_aplicada'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.penalidadeaplicada pea where prc.prcid = pea.prcid) = 0";
    }
    
     if( $dados['ministerio_publico'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.ministeriopublico mp where prc.prcid = mp.prcid) > 0";
    }elseif($dados['ministerio_publico'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.ministeriopublico mp where prc.prcid = mp.prcid) = 0";
    }
    
     if( $dados['in_loco'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.verificacaoinloco il where prc.prcid = il.prcid) > 0";
    }elseif($dados['in_loco'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.verificacaoinloco il where prc.prcid = il.prcid) = 0";
    }
    
     if( $dados['vistas_disponibilizadas'] == "t" ){       
        $arrWhere[] = " (select count(*) from cproc.disponibilidadevistas vd where prc.prcid = vd.prcid) > 0";
    }elseif($dados['vistas_disponibilizadas'] == "f"){
        $arrWhere[] = " (select count(*) from cproc.disponibilidadevistas vd where prc.prcid = vd.prcid) = 0";
    }

    if( $dados['prcstatusead'] == "t" ){       
        $arrWhere[] = " prc.prcstatusead = TRUE ";
    }elseif($dados['prcstatusead'] == "f"){
        $arrWhere[] = " prc.prcstatusead = FALSE ";
    }

    if( $dados['prcstatussupesp'] == "t" ){       
        $arrWhere[] = " prc.prcstatussupesp = TRUE ";
    }elseif($dados['vistas_disponibilizadas'] == "f"){
        $arrWhere[] = " prc.prcstatussupesp = FALSE ";
    }
    
   

    $arrWhere[] = "prc.prcstatus = 'A'";

    if($arrWhere != ""){
        $WHERE = " WHERE " . implode(' AND ', $arrWhere);
    }

    if( in_array(PERFIL_SUPER_USUARIO, $perfil) ){
        $acao = "
            <img src=\"../imagens/alterar.gif\" title=\"Abrir\" id=\"' || prc.prcid ||'\" class=\"alterar\" onclick=\"alterarProcesso('|| prc.prcid ||');\" style=\"cursor:pointer;\"/>
            <img src=\"../imagens/excluir.gif\" title=\"Excluir\" id=\"' || prc.prcid ||'\" class=\"excluir\" onclick=\"excluirProcesso('|| prc.prcid ||');\" style=\"cursor:pointer;\"/>
        ";
    } else {
        $acao = "
            <img src=\"../imagens/alterar.gif\" title=\"Abrir\" id=\"' || prc.prcid ||'\" class=\"alterar\" onclick=\"alterarProcesso('|| prc.prcid ||');\" style=\"cursor:pointer;\"/>
            <img src=\"../imagens/excluir_01.gif\" title=\"Excluir\" id=\"' || prc.prcid ||'\" class=\"excluir\"/>
        ";
    }

    $sql = "
        SELECT DISTINCT  '{$acao}' ||
                CASE WHEN (select count(prcid) from cproc.documento where prcid=prc.prcid ) > 0 THEN
                        '<img src=\"../imagens/anexo.gif\" title=\"Anexo\" class=\"anexo\" style=\"cursor:pointer;\"/>'
                     ELSE
                        ' '
                END AS acao,
                mun.estuf,
                mun.mundescricao,
                (CASE WHEN esd.esdid = 1312 THEN
                        '<font color=red>'||prc.prcnumsidoc||'</font>'
                     ELSE
                        prc.prcnumsidoc
                END) as prcnumsidoc,
                tpr.tprdsc,
                --usucr.usunome as coordenador,
                --usutr.usunome as tecnico,
                (select u1.usunome from cproc.coordenadoresponsavel c1
                inner join seguranca.usuario u1 ON u1.usucpf = c1.usucpf
                where c1.crpstatus = 'A' and prcid=prc.prcid
                group by u1.usunome,c1.crpid
                order by c1.crpid DESC
                limit 1) as coordenador,
                (select u2.usunome from cproc.tecnicoresponsavel t1
                inner join seguranca.usuario u2 ON u2.usucpf = t1.usucpf
                where t1.trpstatus = 'A' and prcid=prc.prcid
                group by u2.usunome,t1.trpid
                order by t1.trpid DESC
                limit 1) as tecnico,
                esd.esddsc,
                (SELECT to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI:SS') FROM workflow.historicodocumento
                 WHERE docid=doc.docid) AS datadocmax,
                (select f.fasdsc from cproc.fase f
                 inner join (select max(fprid) as fprid, fasid, prcid from cproc.faseprocesso group by fasid, prcid) p on p.fasid = f.fasid
                 where prcid=prc.prcid
                 order by fprid DESC
                 limit 1) as fase,
                 (select s.stsdsc from cproc.status s
                 inner join (select max(stpid) as stpid, stsid, prcid from cproc.statusprocesso group by stsid, prcid) st on st.stsid = s.stsid
                 where prcid=prc.prcid
                 order by stpid DESC
                 limit 1) as status

        FROM cproc.processo prc
        LEFT JOIN territorios.municipio mun ON prc.muncod = mun.muncod
        LEFT JOIN cproc.tipoprocesso tpr ON tpr.tprid = prc.tprid
        --LEFT JOIN cproc.coordenadoresponsavel cr ON prc.prcid = cr.prcid AND cr.crpstatus = 'A'
        --LEFT JOIN seguranca.usuario usucr ON cr.usucpf = usucr.usucpf
        --LEFT JOIN cproc.tecnicoresponsavel tr ON prc.prcid = tr.prcid AND tr.trpstatus = 'A'
        --LEFT JOIN seguranca.usuario usutr ON tr.usucpf = usutr.usucpf
        LEFT JOIN workflow.documento doc ON doc.docid = prc.docid
		LEFT JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                
        {$WHERE}
            
        ORDER BY mun.estuf, mun.mundescricao
    ";
        //ver($sql);
    $cabecalho = array('Ação', 'UF', 'Município', 'Nº do Processo SIDOC', 'Tipo de Processo', 'Coordenador', 'Técnico', 'Situação', 'Última Data de Tramitação', 'Fase', 'Status');
    $alinhamento = Array('center', '', '', 'center', '', '', '', '', '');
    $tamanho = Array('5%', '', '', '9%','', '', '', '', '');

    if($tipo=='xls') {

        ob_clean();
        header("Content-type: application/vnd.ms-excel");
        header("Content-type: application/force-download");
        header("Content-Disposition: attachment; filename=simec_lista_cproc.xls");
        header("Pragma: no-cache");
        $db->monta_lista($sql, $cabecalho, 10000, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

    }else{

        $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', 'N', $tamanho, $alinhamento);

    }
}

function excluirProcesso($prcid) {
	global $db;
	if ($prcid != '') {
		$sql = "UPDATE cproc.processo SET prcstatus = 'I' WHERE prcid = {$prcid} ";
	}

	if( $db->executar($sql) ){
		$db->commit();
	}
}


/**
 * Formulario de "atualizaInformacoesProcedimento"
 */
function form_atualizaInformacoesProcedimento(){
    global $db;

    // busca documento
    $sql = " SELECT *
             FROM workflow.documento
             WHERE docid = " . $_POST['docid'];
    $documento = $db->carregar( $sql );
    ?>
    <div>
        <h3>Atualização de Informações de Procedimento</h3>
        <?php //TODO: verificar se pode ser retirado ?>
        <input name="docid" type="hidden" value="<?=$_POST['docid']?>"/>
        <table>
            <tr>
                <td class="subtituloDireita" style="width:20%;">Fase:</td>
                <td>
                    <?php
                        $sql = " SELECT 
                                    f.fasid AS codigo, 
                                    TRIM(f.fasdsc) AS descricao
                                 FROM cproc.fase f
                        ";
                        $db->monta_combo('fasid', $sql, 'S', 'Selecione...', '', '', '', '455', 'S', '', '','','Fase');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Status:</td>
                <td>
                    <?php
                        $sql = " SELECT 
                                    s.stsid AS codigo, 
                                    TRIM(s.stsdsc) AS descricao
                                 FROM cproc.status s
                        ";
                        $db->monta_combo('stsid', $sql, 'S', 'Selecione...', '', '', '', '455', 'S', '', '','','Status');
                    ?>
                </td>
            </tr>
            <?php 
            switch ($documento[0]['esdid']) {
                case CPROC_EMCADASTRAMENTO_ESDID: ?>
                    <tr>
                        <td class="subtituloDireita">Coordenador:</td>
                        <td>
                            <?php
                                $sql = " SELECT DISTINCT  
                                            usuario.usucpf as codigo, 
                                            usuario.usunome as descricao
                                        FROM 
                                            seguranca.perfil perfil 
                                            inner join seguranca.perfilusuario perfilusuario   on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = ".PERFIL_COORDENADOR_CPROC."
                                            right join seguranca.usuario usuario           on usuario.usucpf = perfilusuario.usucpf
                                            inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
                                            left join  entidade.entidade entidade               on usuario.entid = entidade.entid
                                            left join  public.cargo cargo                       on cargo.carid = usuario.carid
                                        WHERE 
                                            usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = ".CPROC_SISID." and (perfil.pflcod = ".PERFIL_COORDENADOR_CPROC.") 
                                        GROUP BY 
                                            usuario.usucpf, usuario.usunome, usuario.usufoneddd, 
                                            usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome, 
                                            usuario.orgao, usuario.usudataatualizacao , cargo.cardsc, usuario.usufuncao
                                        ORDER BY 
                                            descricao
                                ";
                                // ver($sql,d);
                                $db->monta_combo('coordenador', $sql, 'S', 'Selecione...', '', '', '', '455', 'S', '', '','','Coordenador');
                            ?>
                        </td>
                    </tr> 
                    <?php break;
                //case CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID: ?>
                    <!--<tr>
                        <td class="subtituloDireita">Técnico:</td>
                        <td>
                            <?php
                                $sql = " SELECT DISTINCT  
                                            usuario.usucpf as codigo, 
                                            usuario.usunome as descricao
                                        FROM 
                                            seguranca.perfil perfil 
                                            inner join seguranca.perfilusuario perfilusuario   on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = ".PERFIL_TECNICO_CPROC."
                                            right join seguranca.usuario usuario           on usuario.usucpf = perfilusuario.usucpf
                                            inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
                                            left join  entidade.entidade entidade               on usuario.entid = entidade.entid
                                            left join  public.cargo cargo                       on cargo.carid = usuario.carid
                                        WHERE 
                                            usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = ".CPROC_SISID." and (perfil.pflcod = ".PERFIL_TECNICO_CPROC.") 
                                        GROUP BY 
                                            usuario.usucpf, usuario.usunome, usuario.usufoneddd, 
                                            usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome, 
                                            usuario.orgao, usuario.usudataatualizacao , cargo.cardsc, usuario.usufuncao
                                        ORDER BY 
                                            descricao
                                ";
                                // ver($sql,d);
                                $db->monta_combo('tecnico', $sql, 'S', 'Selecione...', '', '', '', '455', 'S', '', '','','Coordenador');
                            ?>
                        </td>
                    </tr>-->
                    <?php //break;

                // 'definirPrazoManifestacao'
                case CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID: ?>
                    <tr>
                        <td class="subtituloDireita" style="width:20%;">Prazo de Manifestação:</td>
                        <td>
                            <?php
                                echo campo_texto('pzmprazomanifestacao', 'N', 'S', 'Prazo', '51', '100', '###', '', '', '', '', 'id="pzmprazomanifestacao"', '', '', '');
                            ?>
                        </td>
                    </tr>
                    <?php break;
            }
            ?>
        </table>
    </div>
    <?php
}

/**
 * Estado Documento: Em Cadastramento - CPROC
 * Pré-Ação de "Enviar para Análise do Procedimento pelo Coordenador"
 */
function atualizaInformacoesProcedimento(){

    return WorkflowCproc::preAcaoEnviarParaAnaliseDoProcedimentoPeloCoordenador();

}


/**
 * Formulário de "definirPrazoAnalise"
 */
function form_definirPrazoAnalise(){
    global $db;

    // busca documento
    $sql = " SELECT *
             FROM workflow.documento
             WHERE docid = " . $_POST['docid'];
    $documento = $db->carregar( $sql );
    ?>
    <div>
        <h3>Definição de Prazo para Análise</h3>
        <table>
            <tr>
                <td class="subtituloDireita" style="width:20%;">Prazo:</td>
                <td>
                    <?php
                        echo campo_texto('crpprazodefinido', 'N', 'S', 'Prazo', '2', '2', '##', '', '', '', '', 'id="crpprazodefinido"', '', '', '');
                    ?> dias
                </td>
            </tr>
                    <tr>
                        <td class="subtituloDireita">Técnico:</td>
                        <td>
                            <?php
                                $sql = " SELECT DISTINCT  
                                            usuario.usucpf as codigo, 
                                            usuario.usunome as descricao
                                        FROM 
                                            seguranca.perfil perfil 
                                            inner join seguranca.perfilusuario perfilusuario   on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = ".PERFIL_TECNICO_CPROC."
                                            right join seguranca.usuario usuario           on usuario.usucpf = perfilusuario.usucpf
                                            inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
                                            left join  entidade.entidade entidade               on usuario.entid = entidade.entid
                                            left join  public.cargo cargo                       on cargo.carid = usuario.carid
                                        WHERE 
                                            usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = ".CPROC_SISID." and (perfil.pflcod = ".PERFIL_TECNICO_CPROC.") 
                                        GROUP BY 
                                            usuario.usucpf, usuario.usunome, usuario.usufoneddd, 
                                            usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome, 
                                            usuario.orgao, usuario.usudataatualizacao , cargo.cardsc, usuario.usufuncao
                                        ORDER BY 
                                            descricao
                                ";
                                // ver($sql,d);
                                $db->monta_combo('tecnico', $sql, 'S', 'Selecione...', '', '', '', '455', 'S', '', '','','Coordenador');
                            ?>
                        </td>
                    </tr>
            </table>
    </div>
    <?php
}

/**
 * Estado Documento: Em Análise de Procedimento pelo Coordenador
 * Pré-Ação de "Designar Técnico Responsável"
 */
function definirPrazoAnalise(){

    WorkflowCproc::preAcaoDesignarTecnicoResponsavel();

}

/**
 * Estado Documento: Em Análise de Procedimento pelo Coordenador
 * Pós-Ação de "Elaborar Minuta"
 */
function uploadMinuta(){

    echo '<script>
        alert("Faça upload do arquivo em seguida.");
        window.opener.location.href = "/cproc/cproc.php?modulo=principal/uploadMinuta&acao=A";
        window.close();
    </script>';

    return true;

}

function UploadDocumento(){

	echo '<script>
	        alert("Faça upload do arquivo em seguida.");
	        window.opener.location.href = "/cproc/cproc.php?modulo=principal/uploadDocumento&acao=A";
	        window.close();
	    </script>';
	
	return true;
	
}

/**
 * Estado Documento: Em Atualização do Procedimento
 * Pré-Ação de "Enviar para Elaboração da minuta pelo técnico"
 */
// função "atualizaInformacoesProcedimento" declarada acima

/**
 * Estado Documento: Em Atualização do Procedimento
 * Pós-Ação de "Enviar para Elaboração da minuta pelo técnico"
 */
 function contarPrazo(){
    //WorkflowCproc::posAcaoEnviarParaElaboracaoDaMinutaPeloTecnico();
	global $db;
	
	$sql = " UPDATE cproc.tecnicoresponsavel
					 SET trpdtfinalprazo = NOW() + interval '1 day' * (
					 	SELECT crpprazodefinido
					 	FROM cproc.coordenadoresponsavel
					 	WHERE prcid = ( SELECT prcid
										FROM cproc.processo
										WHERE docid = '".$_POST['docid']."' )
					 	ORDER BY crpdthrdistribuicao DESC
					 	LIMIT 1
					 )
					 WHERE prcid = ( SELECT prcid
									 FROM cproc.processo
									 WHERE docid = '".$_POST['docid']."' ) ";
	
	$db->executar( $sql );
	
	return $db->commit();
 }

/**
 * Estado Documento: Em Atualização do Procedimento
 * Pré-Ação de "Enviar para Análise do Procedimento pelo Coordenador"
 */
// função "atualizaInformacoesProcedimento" declarada acima

/**
 * Estado Documento: Em Atualização do Procedimento
 * Pré-Ação de "Enviar para Análise da Minuta pelo Coordenador"
 */
// função "atualizaInformacoesProcedimento" declarada acima

/**
 * Estado Documento: Em Elaboração da Minuta pelo técnico
 * Pós-Ação de "Elaborar Minuta"
 */
// função "uploadMinuta" declarada acima


/**
 * Formulário de "definirPrazoExecucao"
 */
function form_definirPrazoExecucao(){
    global $db;

    ?>
    <div>
        <h3>Definição de Prazo para Execução</h3>
        <table>
            <tr>
                <td class="subtituloDireita" style="width:20%;">Prazo:</td>
                <td>
                    <?php
                        echo campo_texto('pzeprazoexecucao', 'N', 'S', 'Prazo', '51', '100', '###', '', '', '', '', 'id="pzeprazoexecucao"', '', '', '');
                    ?>
                </td>
            </tr>
        </table>
    </div>
    <?php
}

/**
 * Estado Documento: Impressão, assinatura e numeração do documento
 * Pré-Ação de "Registrar Publicação"
 */
function definirPrazoExecucao(){

    WorkflowCproc::preAcaoRegistrarPublicacao();

}

/**
 * Estado Documento: Impressão, assinatura e numeração do documento
 * Pós-Ação de "Registrar Publicação"
 */
function enviarEmailTecnicoCoordenador(){

    WorkflowCproc::posAcaoRegistrarPublicacao();

}

/**
 * Estado Documento: Impressão, assinatura e numeração do documento
 * Pré-Ação de "Notificar Interessado"
 */
// função "atualizaInformacoesProcedimento" declarada acima

/**
 * Estado Documento: Impressão, assinatura e numeração do documento
 * Pré-Ação de "Enviar para Aguardar Manifestação do Interessado"
 */
// função "atualizaInformacoesProcedimento" declarada acima

/**
 * Estado Documento: Aguardar Manifestação do Interessado
 * Pré-Ação de "Enviar para Análise dos documentos pelo Coordenador"
 */
// função "atualizaInformacoesProcedimento" declarada acima

/**
 * Estado Documento: Em Análise dos documentos pelo Coordenador
 * Pré-Ação de "Enviar para Arquivar Procedimento"
 */
// função "atualizaInformacoesProcedimento" declarada acima



function responsaveisProcesso($prcid){?>
            <tr>
                <td class="subtituloDireita" rowspan="3" width="20%">Responsáveis:</td>
            </tr>
            <tr>
                <td class="subtituloDireita" width="12%">Coordenador:</td>
                <td><?= ($prcid)?Processo::resgataCoordenador( $prcid ):'' ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Técnico:</td>
                <td><?= ($prcid)?Processo::resgataTecnico( $prcid ):'' ?></td>
            </tr>
            <!-- Situação -->
            <tr>
                <td class="subtituloDireita" rowspan="3" width="20%">Situação:</td>
            </tr>
            <tr>
                <td class="subtituloDireita" width="12%">Fase:</td>
                <td><?= ($prcid)?Processo::resgataFase( $prcid ):''; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Status:</td>
                <td><?= ($prcid)?Processo::resgataStatus( $prcid ):'' ?></td>
            </tr>
    
<?php }

/**
 * Victor Martins Machado
 * Função de inclusão de uma nova entidade
 */
function cadastraEntidade($post){
	global $db;
	
	extract($post);
	
	$sql = "INSERT INTO cproc.entidade (entdsc, entdtinclusao, entdtstatus) VALUES ('{$entdsc}', NOW(), '{$entdtstatus}') RETURNING entid";
	
	$entid = $db->pegaUm($sql);
	
	if($entid){
		$db->commit();
		$parametros = "&acao=A&entid=".$entid;
		$db->sucesso('principal/cad_dados_entidade', $parametros, 'Operação realizada com sucesso!', 'N', 'N');
	} else {
		$db->insucesso('Não foi possível realizar a operação.', '', 'principal/cad_dados_entidade&acao=A');
	}
}

/**
 * Victor Martins Machado
 * Função de alteração de entidade
 */
function alteraEntidade($post){
	global $db;
	
	extract($post);
	
	$sql = "UPDATE cproc.entidade SET
				entdsc = '{$entdsc}',
				entdtstatus = '{$entdtstatus}'
			WHERE entid = {$entid} RETURNING entid";
	
	$entid = $db->pegaUm($sql);
	
	if($entid){
		$db->commit();
		$parametros = "&acao=A&entid=".$entid;
		$db->sucesso('principal/cad_dados_entidade', $parametros, 'Operação realizada com sucesso!', 'N', 'N');
	} else {
		$db->insucesso('Não foi possível realizar a operação.', '', 'principal/cad_dados_entidade&acao=A');
	}
}

/**
 * Victor Martins Machado
 * Função que retorna a entidade a partir do entid
 */
function exibirEntidade($entid){
	global $db;

	if($entid){
		$aryWhere[] = "entid = {$entid}";
	}

	$sql = "SELECT 	entid, entdsc, entdtstatus FROM cproc.entidade ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";
	
	$entidade = $db->pegaLinha($sql);
	
	return $entidade;
}

/**
 * Victor Martins Machado
 * Função que exclui a entidade
 */
function inativarEntidade($entid){
	global $db;
		
	$sql = "UPDATE cproc.entidade SET
                entdtstatus = 'I'
            WHERE entid = {$entid} RETURNING entid";
		
	$ent = $db->pegaUm($sql);
		
	if ($ent){
		$db->commit();
		$parametros = "&acao=A";
		$db->sucesso('principal/cad_dados_entidade', $parametros, 'Operação realizada com sucesso!', 'N', 'N');
	} else {
		$db->insucesso('Não foi possível realizar a operação.', '', 'principal/cad_dados_entidade&acao=A');
	}
}

/**
 * Victor Martins Machado
 * Função que retorna uma tabela com as entidades
 */
function listarEntidade(){
	global $db;
	
		$acao = "
            CASE
            	WHEN entdtstatus = 'A' THEN '<img src=\"../imagens/alterar.gif\" id=\"' || entid ||'\" class=\"alterar\" onclick=\"alteraEntidade('|| entid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;<img src=\"../imagens/excluir.gif\" id=\"' || entid ||'\" class=\"excluir\" onclick=\"inativarEntidade('|| entid ||');\" style=\"cursor:pointer;\"/>'
                WHEN entdtstatus = 'I' THEN '<img src=\"../imagens/alterar.gif\" id=\"' || entid ||'\" class=\"alterar\" onclick=\"alteraEntidade('|| entid ||');\" style=\"cursor:pointer;\"/>'
            END
        ";
	
	$sql = "SELECT 
        		".$acao.",
                entdsc,
                to_char(entdtinclusao, 'dd/mm/yyyy') as entdtinclusao,
                CASE 
        			WHEN entdtstatus = 'A' THEN 'Ativo'
        			WHEN entdtstatus = 'I' THEN 'Inativo'
        		END as entdtstatus
        	FROM cproc.entidade";

	$cabecalho = array('Ação', 'Nome', 'Data de Inclusão', 'Status');
	$alinhamento = Array('center', '', '', '');
	$tamanho = Array('5%', '70%', '', '');
	$db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', 'N', $tamanho, $alinhamento);
}

/**
 * Recupera o(s) perfil(is) do usuário no módulo
 *
 * @return array $pflcod
 */
function arrayPerfil() {
	/*     * * Variável global de conexão com o bando de dados ** */
	global $db;

	/*     * * Executa a query para recuperar os perfis no módulo ** */
	$sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . CPROC_SISID . "
			WHERE
				pu.usucpf = '" . $_SESSION['usucpf'] . "'
			ORDER BY
				p.pflnivel";
	$pflcod = $db->carregarColuna( $sql );

	/*     * * Retorna o array com o(s) perfil(is) ** */
	return (array) $pflcod;
}

/**
 * Retorna a parmissão do usuário no processo
 * @return string
 */
function retornaPermissao($prcid){
	global $db;
	$pfls = arrayPerfil();
	
	if ($prcid){
		// busca documento
		$sql = "SELECT
					d.esdid
				FROM cproc.processo p
				INNER JOIN workflow.documento d ON p.docid = d.docid
				WHERE p.prcid = " . $prcid;
		$documento = $db->carregar( $sql );
	}
		
	if (!empty($documento[0]['esdid']) && $documento[0]['esdid'] != CPROC_EMCADASTRAMENTO_ESDID){
		if (!$_REQUEST['prcid'] && !$_SESSION['cproc']['prcid']){
			if (!in_array(PERFIL_SUPER_USUARIO, $pfls) && !in_array(PERFIL_APOIO_CPROC, $pfls)){
				$cpfusu = '';
			
				if (in_array(PERFIL_COORDENADOR_CPROC, $pfls)){
					$cpfusu = Processo::resgataCpfCoordenador($prcid);
				} else {
					if (in_array(PERFIL_TECNICO_CPROC, $pfls)){
						$cpfusu = Processo::resgataCpfTecnico($prcid);
					}
				}
			
				if ($_SESSION['usucpf'] == $cpfusu){
					return 'S';
				}
			} else {
				return 'S';
			}
		} else {
			return 'N';
		}
	} else {
		return 'S';
	}
}

function retornaPermissaoWorkflow(){
	global $db;
	$pfls = arrayPerfil();
	$prcid = $_SESSION['cproc']['prcid'];
	$cpfusu = '';
	if (!in_array(PERFIL_SUPER_USUARIO, $pfls)){
		if (in_array(PERFIL_APOIO_CPROC, $pfls)){
			$sql = "SELECT
								d.esdid
							FROM cproc.processo p
							INNER JOIN workflow.documento d ON p.docid = d.docid
							WHERE p.prcid = " . $prcid;
			$esdid = $db->carregar( $sql );
			
			if ($esdid[0]['esdid'] != CPROC_EMCADASTRAMENTO_ESDID &&
			$esdid[0]['esdid'] != CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID &&
			$esdid[0]['esdid'] != CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID &&
			$esdid[0]['esdid'] != CPROC_AGUARDARMANIFESTACAOINTERESSADO_ESDID &&
			$esdid[0]['esdid'] != CPROC_ARQUIVARPROCEDIMENTO_ESDID) {
				//return "Teste".$esdid[0]['esdid'].' - '.CPROC_EMCADASTRAMENTO_ESDID.' - '.CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID.' - '.CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID.' - '.CPROC_AGUARDARMANIFESTACAOINTERESSADO_ESDID.' - '.CPROC_ARQUIVARPROCEDIMENTO_ESDID;
				return "Usuário sem permissão.";
			}
		} else {			
			if (in_array(PERFIL_COORDENADOR_CPROC, $pfls)){
				$cpfusu = Processo::resgataCpfCoordenador($prcid);
			} else {
				if (in_array(PERFIL_TECNICO_CPROC, $pfls)){
					$cpfusu = Processo::resgataCpfTecnico($prcid);
				}
			}
				
			if ($_SESSION['usucpf'] != $cpfusu){
				return "Usuário sem permissão.";
			}
		}
	}
	
	return true;
}

function retornaPermissaoDetalhe($prcid){
	global $db;
	$pfls = arrayPerfil();

	if (!in_array(PERFIL_SUPER_USUARIO, $pfls) && !in_array(PERFIL_APOIO_CPROC, $pfls)){
		return 'N';
	} else {
		return 'S';
	}
}

?>