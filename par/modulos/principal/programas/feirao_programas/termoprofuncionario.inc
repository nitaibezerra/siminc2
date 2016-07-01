<?php

unset($_SESSION['par']['pcuid']);

if($_REQUEST['enviaEmail']){

	if(empty($_SESSION['par']['muncod'])){
		/*
		$sql = "select distinct ent.entid, entnumcpfcnpj, entnome, entemail from entidade.entidade ent
				inner join entidade.funcaoentidade fue on fue.entid = ent.entid
				inner join entidade.endereco ede on ede.entid = ent.entid
				where fue.funid = 15
				and ede.muncod = '{$_SESSION['par']['muncod']}'
				and ent.entstatus = 'A'
				and fue.fuestatus = 'A'
				and ede.endstatus = 'A'
				order by ent.entid desc
				limit 1";
		*/
		$sql = "select distinct ent.entid, entnumcpfcnpj, entnome, entemail from par.entidade ent
				INNER JOIN par.instrumentounidade 	iu 	ON iu.inuid = ent.inuid
				INNER JOIN territorios.municipio 	m 	ON m.muncod = iu.muncod
				INNER JOIN territorios.estado 		est ON est.estuf = m.estuf
				WHERE
					iu.muncod = '{$_SESSION['par']['muncod']}' AND ent.dutid = ".DUTID_DIRIGENTE;
		$rs = $db->pegaLinha($sql);

		$arEmail[] = $rs['entemail'];

	}else{
		/*
		$sql = "select distinct ent.entid, entnumcpfcnpj, entnome, entemail from entidade.entidade ent
				inner join entidade.funcaoentidade fue on fue.entid = ent.entid
				inner join entidade.endereco ede on ede.entid = ent.entid
				where fue.funid = 25
				and ede.estuf = '{$_SESSION['par']['estuf']}'
				and ent.entstatus = 'A'
				and fue.fuestatus = 'A'
				and ede.endstatus = 'A'
				order by ent.entid desc
				limit 1";
		*/
		$sql = "select distinct ent.entid, entnumcpfcnpj, entnome, entemail from par.entidade ent
				WHERE ent.estuf = '{$_SESSION['par']['estuf']}' AND ent.dutid = ".DUTID_SECRETARIO_ESTADUAL;

		$rs = $db->pegaLinha($sql);

		$arEmail[] = $rs['entemail'];

	}

	$remetente 	= '';
	$assunto	= 'Conclusão de Adesão ao Pacto';

	$conteudo	= '
					<p>Prezado(a) Secretário(a),</p>

					<p>A sua Secretaria de Educação acaba de concluir a adesão ao Pacto Nacional pela Alfabetização na Idade Certa.
					Em breve, o Ministério da Educação entrará em contato para informar as próximas etapas.
					Para saber mais, acesse o portal do MEC: <a href="http://www.mec.gov.br" target="_blank">http://www.mec.gov.br</a>.</p>
				  ';

	$cc			= array();
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	die;
}

include_once APPRAIZ . "includes/workflow.php";

if($_REQUEST['req']){
    $_REQUEST['req']($_REQUEST);
    exit;
}


//pega termos
if($_SESSION['par']['prgid']){

	if( $_SESSION['par']['estuf'] == 'DF' && $_SESSION['par']['itrid'] == 1 ){
		$tprcod = 2;
	} else {
		$tprcod = $_SESSION['par']['muncod'] == '' ? 1 : 2;
	}

	/*
	if($_SESSION['par']['estuf'] == 'DF' && ($_SESSION['par']['prgid'] == 157 || $_SESSION['par']['prgid'] == 218 || $_SESSION['par']['prgid'] == 220) ){
		$tprcod = 1;
	}
	*/
	$pfaesfera = $db->pegaUm("select pfaesfera from par.pfadesao where pfaid = ".$_SESSION['par']['pfaid']);
	if($_SESSION['par']['estuf'] == 'DF' && $_SESSION['par']['itrid'] == 1 && ($pfaesfera == 'E' || $pfaesfera == 'T')) $tprcod = 1;

	/*
	if($_SESSION['usucpf'] == ''){
		dbg($_SESSION['par']['estuf']);
		dbg($pfaesfera);
		dbg($tprcod);
		dbg($_SESSION['par']['prgid']);
		dbg($_SESSION['par']['pfaid']);
	}
	*/

	$sql = "SELECT
				tap.tapid,
				tap.taptermo,
				tap.tapinstrucao,
				tap.tapmsg,
				tap.tappacto,
				tap.tapmsgaceitetermo,
			    tap.tapmsgaceitepacto,
			    tap.tapmsgnaoaceitetermo,
			    tap.tapmsgnaoaceitepacto,
			    tap.tpdid,
			    tap.taptituloadesao,
			    tap.tapmsgconclusaoadesao
			FROM
				par.pftermoadesaoprograma tap
			WHERE
				tap.prgid = ".$_SESSION['par']['prgid']."
			AND
				tap.tapano = ".$_SESSION['par']['pfaano']."
			AND
				tap.tapstatus = 'A'
			AND
				tap.tprcod = ".$tprcod;

	$dados = $db->pegaLinha($sql);

	if($dados) extract($dados);
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

#Nova regra inserida na data 01/08/2012.
#A função "pegarProgramaDisponivel()" traz apenas os programas que ainda estão em tempo hábil para a adesão.
#A estrutura condicional verifica se o programa esta em data abil pra adesão. Apos a verificação, caso não esteja
#é dada um mensagem um "alert" que o prazo já expirou e que não é mais possivel a adesao e nem a aleração dos dadso apenas a visualização
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
$programa = pegarProgramaDisponivel();
$programa = $programa ? $programa : array();

if($_SESSION['par']['adpid']){
    if( $tapmsgconclusaoadesao && in_array(prePegarEstadoAtual( pgCriarDocumento( $_SESSION['par']['adpid'] ) ), array(WF_TIPO_PNAIC_PREANALISE)) ){
        echo "<script type=\"text/javascript\">
                    jQuery(function(){
                        jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                        jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                        jQuery( '#dialog-content' ).html('{$tapmsgconclusaoadesao}');
                        jQuery( '#dialog-confirm' ).dialog({
                            resizable: false,
                            width: 550,
                            modal: true,
                            buttons: {
                                'Ok': function() {
                                        jQuery( this ).dialog( 'close' );
                                }
                            }
                        });
                    });
          </script>";
    }
}
$abilitaBotaoAceita = 'N'; 	//Autorizando para todos
$habilita_transporte = 'S'; //Adicionado para Transporte Escolar Acessível
        
$sql = "SELECT prgid FROM par.usuarioresponsabilidade WHERE pflcod = 540 AND usucpf = '{$_SESSION['usucpf']}'";

$arrPrgidAnalista = $db->carregarColuna($sql);

$_SESSION['permiteEdicao'] = 'S';

if($programa){
    if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){
            $abilitaBotaoAceita = 'S';
    }

    if(	(
            ($_SESSION['par']['prgid'] == PROG_PAR_ESCOLA_GESTORES && in_array(PROGRAMA_ESCOLA_GESTORES, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_PROFUNCIONARIO && in_array(PROGRAMA_PROFUNCIONARIO, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_PROINFANTIL && in_array(PROGRAMA_PROINFANTIL, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_PRADIME && in_array(PROGRAMA_PRADIME, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_PROCONSELHO && in_array(PROGRAMA_PROCONSELHO, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_ALFABETIZACAO_IDADE_CERTA && in_array(PROGRAMA_ALFABETIZACAO_IDADE_CERTA, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_AGUANAESCOLA && in_array(PROGRAMA_AGUANAESCOLA, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_EJA && in_array(PROGRAMA_EJA, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_EJA_PRONATEC && in_array(PROGRAMA_EJA_PRONATEC, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_CONSELHO_ESCOLAR && in_array(PROGRAMA_CONSELHO_ESCOLAR, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_CORREPULA && in_array(PROGRAMA_CORREPULA, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_PSE && in_array(PROGRAMA_PSE, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_EMI && in_array(PROGRAMA_EMI, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_ESCOLA_TERRA && in_array(PROGRAMA_ESCOLA_TERRA, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_ALFABETIZACAO_ENSINO_MEDIO && in_array(PROGRAMA_ALFABETIZACAO_ENSINO_MEDIO, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_ATLETA2014 && in_array(PROGRAMA_ATLETA2014, $programa)) ||
            ($_SESSION['par']['prgid'] == PROG_PAR_ESCOLANACOPA && in_array(PROGRAMA_ESCOLANACOPA, $programa))
            )
    ){
            $abilitaBotaoAceita = 'S';
    }
}

if( $_SESSION['par']['prgid'] == PROG_PAR_INSTRUTOR_CONSELHO_ESCOLAR && in_array(PROGRAMA_INSTRUTOR_CONSELHO_ESCOLAR, $programa) ){
	 
	
	if( in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ){
		
		#verifica se o usuario e dirigente
	 	$boDirigente = $db->pegaUm("select count(entid) from par.entidade e where e.dutid in (2, 10) and e.entnumcpfcnpj = '{$_SESSION['usucpf']}'");
	 	if( $boDirigente == 0 ){
	 		$abilitaBotaoAceita = 'N';
	 	} else {
	 		$abilitaBotaoAceita = 'S';
	 	}
	 	if( in_array(PAR_PERFIL_PREFEITO, $perfil))
	 	{
	 		$abilitaBotaoAceita = 'S';
	 	}
	} else if( in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil)  && in_array(PROG_PAR_INSTRUTOR_CONSELHO_ESCOLAR, $arrPrgidAnalista) ){
		$abilitaBotaoAceita = 'S';
	}
}

//NOVA REGRA PAR VERIFICAR A COTA DE ÔNIBUS
if($_SESSION['par']['prgid'] == PROG_PAR_ONIBUS_ACESSIVEL && in_array(PROGRAMA_ONIBUS_ACESSIVEL, $programa)){
        if( $_SESSION['par']['itrid'] == 1 ){ //estadual
                $w = " AND ppaid in (405, 413, 414, 415)";
                $crtid = "(450, 451, 452, 453)";
                $ppsid = 1075;
                if( $_SESSION['par']['estuf'] == 'DF' ){
                        $sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.mun_estuf = map.estuf WHERE iu.mun_estuf = '".$_SESSION['par']['estuf']."'";
                } else {
                        $sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.estuf = map.estuf WHERE iu.estuf = '".$_SESSION['par']['estuf']."' AND iu.itrid = 1";
                }
                $qntPermitida = $db->pegaUm( $sql );
        } elseif( $_SESSION['par']['itrid'] == 2 ){ //municipal
                $w = " AND ppaid in (379, 380, 381, 382)";
                $crtid = "(381, 382, 383, 384)";
                $ppsid = 1074;
                $sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.muncod = map.muncod WHERE iu.muncod = '".$_SESSION['par']['muncod']."' AND iu.itrid = 2";
                $qntPermitida = $db->pegaUm( $sql );
        }

        $sql = "SELECT s.sbaid
                        FROM par.subacao s
                        INNER JOIN par.acao a ON a.aciid = s.aciid
                        INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']} AND s.sbastatus = 'A'
                        WHERE s.ppsid = {$ppsid}";

        $sbaid = $db->pegaUm($sql);

        if(!empty($sbaid)){
                $sql_cotaaderido = "SELECT  	CASE WHEN f.sbaid IS NULL THEN 0 ELSE (COALESCE(SUM(icoquantidadetecnico),0)) END AS qtdAderido
                                                        FROM 		par.subacaoitenscomposicao i
                                                        LEFT JOIN 	( SELECT 		DISTINCT sbaid
                                                                                  FROM 			par.empenhosubacao es
                                                  INNER JOIN 	par.empenho e ON e.empid = es.empid and empstatus = 'A'
                                                  WHERE 		eobstatus = 'A' AND e.empsituacao = '2 - EFETIVADO' AND eobano = '2012') AS f ON f.sbaid = i.sbaid
                                                        WHERE 		i.sbaid = {$sbaid} AND i.icoano = 2012 AND i.icovalidatecnico = 'S'
                                                        GROUP BY  	f.sbaid";

                $cota_aderido = $db->pegaUm($sql_cotaaderido);
        } else {
                $cota_aderido = 0;
        }

        $nova_cota = $qntPermitida - $cota_aderido;

        if(
        	$nova_cota != 0 && 
        	(
        		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
        		in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
                in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) || 
        		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || ( in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil) && in_array(PROG_PAR_ONIBUS_ACESSIVEL, $arrPrgidAnalista) ) 
          	)
    	){
                $abilitaBotaoAceita = 'S';
        }

        if($nova_cota <= 0){
                $habilita_transporte = 'N';
        }
}
if($habilita_transporte == 'N'){
    echo "<script type=\"text/javascript\">
            jQuery(function(){
                    jQuery('#dialog:ui-dialog').dialog('destroy');
                    jQuery('#dialog-confirm').attr({'title':'AVISO'});
                    jQuery('#dialog-content').html('Não existe cota para aquisição de ônibus, para este município!');
                    jQuery('#dialog-confirm').dialog({
                            modal: true,
                            resizable: false,
                            draggable: false,
                            width: 550,
                                    buttons: {
                                    'OK': function() {
                                            jQuery(this).dialog('close');
                                            history.back(-1);
                                    }
                            }
                    });
            });
    </script>";

}elseif(!$_SESSION['par']['adpid'] && $abilitaBotaoAceita == 'S'){
    echo "<script type=\"text/javascript\">
            jQuery(function(){
                jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                jQuery( '#dialog-content' ).html('{$tapmsg}');
                jQuery( '#dialog-confirm' ).dialog({
                    resizable: false,
                    width: 550,
                    modal: true,
                    buttons: {
                        'Ok': function() {
                            jQuery( this ).dialog( 'close' );
                        },
                        'Cancel': function() {
                            jQuery( this ).dialog( 'close' );
                            window.location.href='par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
                        }
                    }
                });
            });
      </script>";

}elseif(!$_SESSION['par']['adpid'] && $abilitaBotaoAceita != 'S'){
        if($_SESSION['par']['prgid'] != 247){
        echo "<script type=\"text/javascript\">
                    jQuery(function(){
                        jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                        jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                        jQuery( '#dialog-content' ).html('O prazo para adesão ao programa já expirou!');
                        jQuery( '#dialog-confirm' ).dialog({
                            resizable: false,
                            width: 550,
                            modal: true,
                            buttons: {
                                'Ok': function() {
                                    jQuery( this ).dialog( 'close' );
                                    history.back(-1);
                                }
                            }
                        });
                    });
          </script>";
        }
}


//programa agua na escola (prgid = 183)
//verifica se atingiu a meta de 1000 escolas aderidas.
/*
if($_SESSION['par']['prgid'] == PROG_PAR_AGUANAESCOLA){
        $sql = "SELECT count(pfa.paeid) as total
                        FROM par.pfaguaescola pfa
                        WHERE pfa.paeparticipa = 'S'";
        $totalEscolasAgua = $db->pegaUm($sql);

        if($totalEscolasAgua >= 1000 && !$_SESSION['par']['adpid']){
                echo "<script type=\"text/javascript\">
                        jQuery(function(){
                                jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                                jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                                jQuery( '#dialog-content' ).html('Não é possível aderir ao programa Água na Escola, pois sua meta já atingiu 1000 Escolas aderidas!');
                                jQuery( '#dialog-confirm' ).dialog({
                                        resizable: false,
                                        width: 550,
                                        modal: true,
                                        buttons: {
                                                'Ok': function() {
                                                        jQuery( this ).dialog( 'close' );
                                                        window.location.href='par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
                                                }
                                        }
                                });
                        });
          </script>";
        }
}
*/
//fim verificação


$verificaPacto  = testaAdesaoPacto();
$verificaAdesao = testaAdesaoProFuncionario();

if( $verificaAdesao != 'S' || ($verificaPacto != 'S' && !empty($tappacto)) ):

	$titulo_modulo = "Adesão ao Programa";
	if($taptituloadesao){
		$titulo_modulo = $taptituloadesao;
	}
	if($verificaPacto != 'S' && !empty($tappacto)){
		$titulo_modulo = "Adesão ao Pacto";
	}
	
	monta_titulo($titulo_modulo,'');
	
	if ($_SESSION['par']['prgid'] == PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA) {
		echo '<br>';
	    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A');
	}
	
	if ($_SESSION['par']['prgid'] == PROG_PAR_EJA_PRONATEC) {
		echo '<br>';
	    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A');
	}
	
	if( $verificaPacto == 'N' ){
	
		$msg = 'Sua Secretaria de Educação não aderiu ao Termo de Pactuação. Confirma?';
	    if(!empty($tapmsgnaoaceitepacto)){
	    	$msg = $tapmsgnaoaceitepacto;
		}
	
	    if(!$_REQUEST['alerta']){
	    	echo "
	        		<script>
	                	jQuery(function(){
	                    	jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
	                        jQuery( '#dialog-content' ).html('{$msg}');
	                        jQuery( '#dialog-confirm' ).dialog({
	                        	resizable: false,
	                            width: 600,
	                            modal: true,
	                            buttons: {
	                            	'Sim': function() {
	                                	jQuery( this ).dialog( 'close' );
	                                },
	                                'Não': function() {
	                                	jQuery( this ).dialog( 'close' );
	                                    document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
	                                }
	                            }
	                        });
	                    });
	                </script>";
		}
	}
	
	if ($verificaAdesao == 'N') {
		$entidade = $tprcod == 1 ? "Estado" : "Município";
	
	    $msg = "Este {$entidade} não aderiu a este programa.";
	    if (!empty($tapmsgnaoaceitetermo)) {
	    	$msg = $tapmsgnaoaceitetermo;
		}
	
	    if (!$_REQUEST['alerta']) {
	    	echo "
	        	<script>
	            	jQuery(function(){
	                	jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
	                    jQuery( '#dialog-content' ).html('{$msg}');
	                    jQuery( '#dialog-confirm' ).dialog({
	                    	resizable: false,
	                        width: 600,
	                        modal: true,
	                        buttons: {
	                        	'Sim': function() {
	                            	jQuery( this ).dialog( 'close' );
	                            },
	                            'Não': function() {
	                            	jQuery( this ).dialog( 'close' );
	                                document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
	                            }
	                        }
	                   	});
	            	});
				</script>";
		}
	}
	
	if(!$_SESSION['par']['prgid']){
		echo '<script type="text/javascript">
	    			alert("Sessão Expirou.\nFavor selecione o programa novamente!");
	        		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
				</script>';
		die;
	}
	?>
	<script>
	
	function validaForm( resposta ){
		var form  = document.getElementById('form');
	    var pacto = document.getElementById('pacto').value;
	
	    if(pacto == 'true'){
	    	document.getElementById('pacto').value = resposta;
	    }else{
	    	document.getElementById('resposta').value = resposta;
	    }
	    document.getElementById('req').value = 'respondeTermoProFuncionario';
	    form.submit();
	}
	
	function cancela(){
		var form  = document.getElementById('form');
		document.getElementById('req').value = 'cancelaTermoProFuncionario';
		form.submit();
	}
	
	function continuar(){
		var form = document.getElementById('form');
		document.getElementById('continua').value = 'S';
		form.submit();
	}
	</script>
	<form id="form" name="form" method="POST">
        <input type="hidden" id="pacto" name="pacto" value="<?php echo ($verificaPacto != 'S' && !empty($tappacto)) ? 'true' : '' ?>"/>
            <input type="hidden" id="resposta" name="resposta" value=""/>
            <input type="hidden" id="continua" name="continua" value=""/>
            <input type="hidden" id="req" name="req" value=""/>
            <input type="hidden" id="tapid" name="tapid" value="<?= $tapid ?>"/>
            <center>
                <table bgcolor="#f5f5f5" align="center" class="tabela" >
                    <tr>
                        <td width="90%" align="center">
                            <?php
                            if ($_SESSION['par']['prgid'] == PROG_PAR_CORREPULA) {
                                ?>
                                <B>INSTRUÇÕES</B>
                                <div style="overflow: auto; height:150px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
                                    <div style="width: 95%; magin-top: 10px; ">
                                        <?= $tapinstrucao ?>
                                    </div>
                                </div>
                                <br />
                                <B>TERMO</B>
                                <?php
                            }

                            ?>
                            <div style="overflow: auto; height:350px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
                                <div style="width: 95%; magin-top: 10px; ">
                                    <br>
                                        <?php
                                            if ($verificaPacto != 'S' && !empty($tappacto)) {

                                                $tappacto = preencheCamposTexto($tappacto);
                                                echo $tappacto;
                                            } else {

                                                $taptermo = preencheCamposTexto($taptermo);
                                                echo $taptermo;
                                            }
                                        ?>
                                </div>
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <?php
                        //ver($verificaAdesao, $abilitaBotaoAceita, d );
                        if ( $_SESSION['par']['prgid'] != PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA && $_SESSION['par']['prgid'] != PROG_PAR_EJA_PRONATEC){ ?>
                                <td align="center">
                                    <div style="float: center; width: 70%; ">
                                        <?php if ($verificaAdesao != 'S' && $abilitaBotaoAceita == 'S'): ?>
                                            <input type="button" name="aceito" value="Aceito" onclick="validaForm('S')"/>
                                        <?php endif; ?>
                                        <?php if ($verificaAdesao == 'S' && $abilitaBotaoAceita == 'S'): ?>
                                            <input type="button" name="cancelar" value="Cancelar Adesão" onclick="cancela()"/>
                                            <input type="button" name="continua" value="Continua" onclick="continuar()"/>
                                        <?php endif; ?>
                                        <?php if ($verificaAdesao != 'S' && $abilitaBotaoAceita == 'S'): ?>
                                            <input type="button" name="naceito" value="Não Aceito" onclick="validaNaoAceito()"/>
                                        <?php endif; ?>
                                        <input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
                                    </div>
                                </td>
                        <?php }else{ ?>
                                <td align="center">
                                    <div style="float: center; width: 70%; ">
                                        <?php if ($verificaAdesao != 'S' && $abilitaBotaoAceita == 'S'): ?>
                                            <input type="button" name="aceito" value="Iniciar Processo" onclick="validaForm('S')"/>
                                            <input type="button" name="naceito" value="Cancelar Processo" onclick="validaNaoAceito()"/>
                                        <?php endif; ?>
                                        <input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
                                    </div>
                                </td>
                        <?php } ?>
                    </tr>
                </table>
            </center>
        </form>
<?php 
else: 

	$sql = "select prgdsc from par.programa where prgid = ".$_SESSION['par']['prgid'];
	$nomePrograma = $db->pegaUm($sql);

	if($_SESSION['par']['muncod']){

		$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
		$rsMunicipio = $db->pegaLinha($sql);
		$descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];

	}else{

		$descricao = $_SESSION['par']['estuf'];
	}

	monta_titulo($nomePrograma,$descricao);

endif; 

function testaAdesaoProFuncionario(){
    global $db;

    if($_SESSION['par']['adpid']){
        $sql = "
            SELECT  max(adpid),
                    adpresposta
            FROM par.pfadesaoprograma
            WHERE adpid = ".$_SESSION['par']['adpid']."

            GROUP BY adpid, adpresposta
            ORDER BY adpid DESC
            LIMIT 1
        ";

        $adpresposta = $db->pegaLinha($sql);
        return $adpresposta['adpresposta'];

    }else{
        return '';
    }
}

function testaAdesaoPacto()
{
	global $db;

	if($_SESSION['par']['adpid']){

		$sql = "SELECT
					max(adpid),
					adprespostapacto
				FROM
					par.pfadesaoprograma
				WHERE
					adpid = ".$_SESSION['par']['adpid']."
				GROUP BY
					adpid,
					adprespostapacto
				ORDER BY
					adpid DESC
				LIMIT 1";

		$adpresposta = $db->pegaLinha($sql);

		return $adpresposta['adprespostapacto'];

	}else{

		return '';
	}
}

function preencheCamposTexto($texto) {
    global $db;
    
    if (strpos($texto, '{secretario}')) {
    	$sql = "SELECT * FROM par.entidade WHERE entstatus = 'A' AND estuf = '{$_SESSION['par']['estuf']}' AND dutid = ".DUTID_SECRETARIO_ESTADUAL;
    	/*
        $sql = "
            select * from entidade.entidade where entid = (
                        SELECT
                                max(ent.entid) as entid1
                        FROM entidade.entidade ent
                                INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 25 AND fue.fuestatus = 'A'
                                INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
                                LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
                                LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid
                                LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid
                                LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 6 AND fue2.fuestatus = 'A'
                                LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
                        WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
                        and eed2.estuf = '{$_SESSION['par']['estuf']}'
                    )
        ";
		*/
        $rs = $db->pegaLinha($sql);
        $texto = str_replace(array('{secretario}', '{cpfsecretario}'), array($rs['entnome'], formatar_cpf($rs['entnumcpfcnpj'])), $texto);
    }

    if (strpos($texto, '{prefeito}')) {
    	$sql = "SELECT * FROM par.entidade ent
		    	INNER JOIN par.instrumentounidade 	iu 	ON iu.inuid = ent.inuid
				INNER JOIN territorios.municipio 	m 	ON m.muncod = iu.muncod
				WHERE
					ent.entstatus = 'A' AND
					iu.muncod = '{$_SESSION['par']['muncod']}' AND ent.dutid = ".DUTID_PREFEITO;
    	/*
        $sql = "
            select distinct ent.entid, entnumcpfcnpj, entnome, entemail
            from entidade.entidade ent
            INNER join entidade.funcaoentidade fue on fue.entid = ent.entid
            INNER JOIN entidade.funcao fun ON fun.funid = fue.funid
            LEFT JOIN entidade.funentassoc fea ON fea.fueid = fue.fueid
            LEFT join entidade.endereco ede on ede.entid = fea.entid
            where fue.funid = 2
            and ede.muncod = '{$_SESSION['par']['muncod']}'
            and ent.entstatus = 'A'
            and fue.fuestatus = 'A'
            and ede.endstatus = 'A'
            order by ent.entid desc
            limit 1
        ";
		*/
        $rs = $db->pegaLinha($sql);
        $texto = str_replace(array('{prefeito}', '{cpfprefeito}'), array($rs['entnome'], formatar_cpf($rs['entnumcpfcnpj'])), $texto);
    }

    if (strpos($texto, '{secretariomunicipal}')) {

    	$sql = "SELECT * FROM par.entidade ent
		    	INNER JOIN par.instrumentounidade 	iu 	ON iu.inuid = ent.inuid
				INNER JOIN territorios.municipio 	m 	ON m.muncod = iu.muncod
				WHERE
					ent.entstatus = 'A' AND
					iu.muncod = '{$_SESSION['par']['muncod']}' AND ent.dutid = ".DUTID_DIRIGENTE;
    	/*
        $sql = "select * from entidade.entidade where entid = (
                        SELECT
                                max(ent.entid) as entid1
                        FROM entidade.entidade ent
                                INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 15 AND fue.fuestatus = 'A'
                                INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
                                LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
                                LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid
                                LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid
                                LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 7 AND fue2.fuestatus = 'A'
                                LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
                        WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
                        and eed2.muncod = '{$_SESSION['par']['muncod']}')";
		*/
        $rs = $db->pegaLinha($sql);
        $texto = str_replace(array('{secretariomunicipal}', '{cpfsecretariomunicipal}'), array($rs['entnome'], formatar_cpf($rs['entnumcpfcnpj'])), $texto);
    }

    if (strpos($texto, '{municipio}')) {

        $sql = "
            SELECT
                mundescricao,
                estuf
            from
                territorios.municipio
            where
                muncod = '{$_SESSION['par']['muncod']}'
        ";
        $rs = $db->pegaLinha($sql);
        $texto = str_replace(array('{municipio}'), array($rs['mundescricao'] . '/' . $rs['estuf']), $texto);
    }

    if (strpos($texto, '{estado}')) {

        $sql = "select
                        estdescricao,
                        estuf
                from
                        territorios.estado
                where
                        estuf = '{$_SESSION['par']['estuf']}'";

        $rs = $db->pegaLinha($sql);

        if (in_array($rs['estuf'], array('AC', 'AM', 'AP', 'CE', 'DF', 'ES', 'GO', 'MA', 'MS', 'MT', 'PA', 'PI', 'PR', 'RJ', 'RN', 'RS'))) {
            $rs['estdescricao'] = 'do ' . $rs['estdescricao'];
        } else if (in_array($rs['estuf'], array('AL', 'MG', 'PE', 'RO', 'RR', 'SC', 'SE', 'SP', 'TO'))) {
            $rs['estdescricao'] = 'de ' . $rs['estdescricao'];
        } else {
            $rs['estdescricao'] = 'da ' . $rs['estdescricao'];
        }

        $texto = str_replace(array('{estado}'), array($rs['estdescricao']), $texto);
    }

    if (strpos($texto, '{dataextenso}')) {

        include_once APPRAIZ . "includes/classes/dateTime.inc";
        $dataClass = new Data();

        $dataExtenso = $dataClass->formataData(date('Y-m-d'), 'DD de mesTextual de YYYY');

        $texto = str_replace(array('{dataextenso}'), array($dataExtenso), $texto);
    }

    if (strpos($texto, '{local}')) {

        if ($_SESSION['par']['muncod']) {

            $sql = "select
                            mundescricao,
                            estuf
                    from
                            territorios.municipio
                    where
                            muncod = '{$_SESSION['par']['muncod']}'";

            $rs = $db->pegaLinha($sql);

            $local = $rs['mundescricao'] . '/' . $rs['estuf'] . ', ';
        } else {

            $local = '';
        }

        $texto = str_replace(array('{local}'), array($local), $texto);
    }

    if( strpos($texto, '{secretariodemandas}') && strpos($texto, '{matriculasiapedemandas}') ){
        if( $_SESSION['par']['adpid'] != '' ){
            $sql = "
                SELECT  TRIM(epsnome) AS epsnome,
                        TRIM(epsmatricula) AS epsmatricula
                FROM eja.ejapronatecsupervisor

                WHERE adpid = '{$_SESSION['par']['adpid']}'
            ";
            $rs = $db->pegaLinha($sql);
        }
        $texto = str_replace(array('{secretariodemandas}', '{matriculasiapedemandas}'), array($rs['epsnome'], $rs['epsmatricula']), $texto);
    }
    
    if(strpos($texto, '{municipioEstadoCnpj}')) {
        if ($_SESSION['par']['inuid']) {
            $sql = "
            SELECT
                inucnpj
            FROM 
                par.instrumentounidade
            WHERE
                  inuid = '{$_SESSION['par']['inuid']}'
            ";
            $inucnpj = $db->pegaUm($sql);
            $texto = str_replace('{municipioEstadoCnpj}', formatar_cnpj($inucnpj), $texto);
        }
    }
    
    if (strpos($texto, '{ano}')) {
        $texto = str_replace('{ano}', date('Y'), $texto);
    }
    
    if (strpos($texto, '{mes}')) {
        $listaMes = array("janeiro","fevereiro","março","abril","maio","junho","julho","agosto","setembro","outubro","novembro","dezembro");
        $texto = str_replace('{mes}', $listaMes[date('m')-1], $texto);
    }
    
    if (strpos($texto, '{dia}')) {
        $texto = str_replace('{dia}', date('d'), $texto);
    }

    return $texto;
}
?>
<style>
    .hidden{display:none}
    .fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
</style>

<?PHP
    //recupera justificativa
    if($_SESSION['par']['adpid']){
        $adpjustificativaresposta = $db->pegaUm("select adpjustificativaresposta from par.pfadesaoprograma where adpid = ".$_SESSION['par']['adpid']);
    }

    $html = '
        <form id="form_just" name="form_just" method=post>
                <input type="hidden" id="pacto" name="pacto" value="'.(($verificaPacto != 'S' && !empty($tappacto)) ? 'true' : '').'"/>
                <input type="hidden" name="resposta" value="N">
                <input type="hidden" name="req" value="respondeTermoProFuncionario">
                <input type="hidden" id="tapid" name="tapid" value="'.$tapid.'"/>
                <center><b>Informe a justificativa para não aceito deste programa.</b></center>
                <br>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td width="30%" valign="top" class="SubtituloDireita" >Justificativa:</td>
                        <td>'.campo_textarea("adpjustificativaresposta", "S", "S", "", 60, 8, 4000).'</td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" ></td>
                        <td class="SubtituloEsquerda" >
                            <input type="button" name="btn_salvar_just" value="Salvar" />
                            <input type="button" name="btn_cancelar_just" value="Cancelar" />
                        </td>
                    </tr>
                </table>
        </form>
    ';
    popupAlertaGeral($html,"500px","280px","div_just","hidden"); ?>

<script>

    jQuery(function() {
        //jQuery('[id=div_just]').hide();

        jQuery('[name=btn_salvar_just]').click(function() {
            var erro = 0;
            if(!jQuery('[name=adpjustificativaresposta]').val()){
                alert('Favor informar a justificativa!');
                erro = 1;
                return false;
            }
            if(erro == 0){
                jQuery('[name=btn_salvar_just]').val("Carregando...");
                jQuery('[name=btn_cancelar_just]').val("Carregando...");
                jQuery('[name=btn_salvar_just]').attr("disabled","disabled");
                jQuery('[name=btn_cancelar_just]').attr("disabled","disabled");
                jQuery('[name=form_just]').submit();
            }
        });

        jQuery('[name=btn_cancelar_just]').click(function() {
            jQuery('[id=div_just]').hide();
        });

    });

    function validaNaoAceito(){
        if(jQuery('pacto').val() == ''){
            jQuery('[id=div_just]').show();
            jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
        }else{
            validaForm('N');
        }
    }

</script>