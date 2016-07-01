<?php

// Verifica Código do termo
verificaCodigoTermoSessao();

$sql = "select * from monitora.previsaoorcamentaria po
		join monitora.pi_planointerno pi on pi.pliid = po.pliid
		where tcpid = {$_SESSION['elabrev']['tcpid']}
		-- retira elementos já enviados atraves de verificacao em elabrev.previsaoparcela
		and po.proid not in(			
			select pre.proid from elabrev.previsaoparcela pre 
			join monitora.previsaoorcamentaria pro on pro.proid = pre.proid
			where pro.tcpid = {$_SESSION['elabrev']['tcpid']}
		)";
//ver($sql, d);
$rsPO = $db->carregar($sql);

if ($_REQUEST['requisicao'] == 'atualizaObs') {
	$sql = "select distinct obscod as codigo, obscod as descricao from elabrev.dadosprogramasfnde where eventocontabil = '300300' and prgcodfnde = '{$_REQUEST['prgcodfnde']}' order by obscod";
	$db->monta_combo('tcpobsfnde', $sql, 'S', '', '', '', '', '', 'S', '', '', 'C7');
	die;
}

if ($_REQUEST['requisicao'] == 'enviarNC') {

    //ver($_REQUEST, d);

	// rotina para retirar campos nao checados para envio
	$camposPorVerificar = array('prgid','espid','esfid','prgfonterecurso');
	foreach ($camposPorVerificar as $value) {
        if (!is_array($_REQUEST[$value])) {
            continue;
        }
		foreach ($_REQUEST[$value] as $key => $celula) {
			if (!in_array($key, $_REQUEST['chekCel']))
				unset($_REQUEST[$value][$key]);
		}
	}

	set_time_limit(30000);
	ini_set("memory_limit", "3000M");
	
	require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

    if($_SERVER['HTTP_HOST']=='simec.mec.gov.br'){
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/nc';
		//echo "Ambiente: Produção";
	}else{
		$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/nc';
		//echo "Ambiente: Desenvolvimento";
	}
// 	ver($_REQUEST, $urlWS,d);
	
	$sqlCnpj = "
		select ungcnpj from monitora.termocooperacao t
		join unidadegestora u on u.ungcod = t.ungcodproponente
		where t.tcpid = {$_SESSION['elabrev']['tcpid']}
		";
	
	$arrParamProcesso['nu_processo'] 		= $_REQUEST['tcpnumprocessofnde'];
	$arrParamProcesso['tp_processo'] 		= '1';
	$arrParamProcesso['co_programa_fnde'] 	= $_REQUEST['tcpprogramafnde'];
	$arrParamProcesso['nu_sistema']			= '';	
	$arrParamProcesso['nu_cnpj_favorecido'] = $db->pegaUm($sqlCnpj);
	
	$arqXml = montaXmlNotaCreditoFnde();
// 	$arqXml = montaXmlNotaCreditoFndeTeste();
	
	if($arqXml && $rsPO){
	
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

        // -- Antes de processar o retorno da requisição, grava o log
        $logRequisicaoFNDE = <<<DML
INSERT INTO elabrev.ws_log(wslrequisicaoxml, wslrespostaxml, metodo, url, wslusucpf)
  VALUES('%s', '%s', '%s', '%s', '%s')
DML;
        $logRequisicaoFNDE = sprintf(
            $logRequisicaoFNDE,
            str_replace("'", "''", $arqXml),
            str_replace("'", "''", $xml),
            'financeiro.nc.solicitar',
            $urlWS,
            $_SESSION['usucpf']
        );
        $db->executar($logRequisicaoFNDE);
        $db->commit();

        // -- Processamento do retorno do XML
		$xmlRetorno    = $xml;
		$xml           = simplexml_load_string( stripslashes($xml) );
		$identificador = (integer) $xml->body->identificador;
		$erroText      = (string) str_replace(array("'"), '', $xml->status->error->message->text);
		$erroCod       = (string) $xml->status->error->message->code;
		$erro          = str_replace(array("'", '"'),'',simec_json_encode($xml->status->error));
		
		$txt = utf8_decode("Erro: {$erroCod} - {$erroText}");


        if (false != $identificador) {

            /*$sql = "update monitora.termocooperacao set
                    codsigefnc = '{$identificador}',
                    retornosigefnc = FALSE
                    where tcpid = {$_SESSION['elabrev']['tcpid']}";*/

            $sql = "UPDATE monitora.previsaoorcamentaria SET
                        sigefid = '{$identificador}',
                        codsigefnc = null
                    WHERE proid IN (".implode(',', $_REQUEST['chekCel']).")";

            $db->executar($sql);
            $db->commit();

            $sql = "insert into elabrev.logtermowssigef (tcpid, logdsc, logxmlenvio, logxmlretorno) values ({$_SESSION['elabrev']['tcpid']}, 'Sucesso', '".str_replace(array("'", '"'),"",$arqXml)."', '".str_replace(array("'", '"'), "", $xmlRetorno)."')";
            $db->executar($sql);
            $db->commit();

            unset($_SESSION['error']['ws']['sigef']['nc']);
            $db->sucesso('principal/termoCooperacao/cadTermoCooperacao', '&aba=tramite');

        } else {
            $sql = "insert into elabrev.logtermowssigef (tcpid, logdsc, logxmlenvio, logxmlretorno) values ({$_SESSION['elabrev']['tcpid']}, '{$erro}', '".str_replace("'","\'",$arqXml)."', '".str_replace("'", "\'", $xmlRetorno)."')";
            $db->executar($sql);
            $db->commit();
            $_SESSION['error']['ws']['sigef']['nc'] = utf8_decode($txt);
        }
	}

	echo "<script>
				alert('Falha no envio da NC, tente novamente!');
				//document.location.href = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=enviarNCfnde';
			</script>";
}

$sql = "select 
			*
		from monitora.termocooperacao tcp 
		where tcp.tcpid = {$_SESSION['elabrev']['tcpid']}";

$rs = $db->pegaLinha($sql);

if($rs) extract($rs);
// excluindo valores
	unset($tcpnumtransfsiafi);


$unicod = $unicod ? $unicod : '26298';
$ungcodemitente = $ungcodemitente ? $ungcodemitente : '153173';
$gescodemitente = $gescodemitente ? $gescodemitente : '15253';
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
	$(function(){
		$('.enviarWS').click(function(){

			// trata checkbox
			var continua = false;
            var checkTrue = [];
			$.each($('[name^="chekCel"]'),function(index,value){ 
				if( $(value).is(':checked') == true ){
                    checkTrue.push($(value).val());
                    //console.log("valor: "+ $(value).val());
					continua = true;
				}
			});
			if(!continua){
				alert('Pelo menos 1 célula orçamentária deve estar marcada para o envio.');
				return false;
			}

			if($('[name=sigefusername]').val() == ''){
				alert('O campo Usuário do SIGEF é obrigatório!');
				$('[name=sigefusername]').focus();
				return false;
			}
			if($('[name=sigefpassword]').val() == ''){
				alert('O campo Senha do SIGEF é obrigatório!');
				$('[name=sigefpassword]').focus();
				return false;
			}
			
			if($('[name=tcpnumtransfsiafi]').val() == ''){
				alert('O campo Número de Transferência do Termo é obrigatório!');
				$('[name=tcpnumtransfsiafi]').focus();
				return false;
			}

			if($('[name=tcpnumprocessofnde]').val() == ''){
				alert('O campo Processo do Termo é obrigatório!');
				$('[name=tcpnumprocessofnde]').focus();
				return false;
			}

			if($('[name=tcpprogramafnde]').val() == ''){
				alert('O campo Programa do Termo é obrigatório!');
				$('[name=tcpprogramafnde]').focus();
				return false;
			}

			if($('[name=tcpobsfnde]').val() == ''){
				alert('O campo Observação do Termo é obrigatório!');
				$('[name=tcpobsfnde]').focus();
				return false;
			}

			if($('[name=tcpobscomplemento]').val() == ''){
				alert('O campo Complemento da Observação é obrigatório!');
				$('[name=tcpobscomplemento]').focus();
				return false;
			}

            console.log(checkTrue);

			retorno_programa = true;
			$("[name^='prgid']").each(function(i, v){
                /*if($(v).val() == ''){
                    retorno_programa = false;
                }*/

                for (var y in checkTrue) {
                    if ($("[name='prgid["+checkTrue[y]+"]']").val()=='') {
                        retorno_programa = false;
                    }
                }
			});
			if(!retorno_programa){
				alert('Existe(m) Célula(s) Orçamentaria(s) em branco, favor preencher os campos.');
				return false;
			}

			retorno_fonte = true;
			$("[name^='prgfonterecurso']").each(function(i, v){
                for (var y in checkTrue) {
                    if ($("[name='prgfonterecurso["+checkTrue[y]+"]']").val()=='') {
                        retorno_fonte = false;
                    }
                }
                /*
				if( $(v).parent().parent().find('[name^=chekCel]').is(':checked') == true ){
					if($(v).val() == ''){
						retorno_fonte = false;			
					}
				}*/
			});
			if(!retorno_fonte){
				alert('Existe(m) Fonte de Recurso(s) em branco, favor preencher os campos.');
				return false;
			}

			$('[name=requisicao]').val('enviarNC');
			$('#formulario').submit();
		});

		$('[name=tcpprogramafnde]').change(function(){
			$.ajax({
				url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=enviarNCfnde',
				type	: 'post',
				data	: 'requisicao=atualizaObs&prgcodfnde='+this.value,
				success	: function(e){
					$('#td_combo_obs').html(e);
				}
			});
		});

	});

    function desabilitaEnvioNc(){
        setTimeout(function(){
            $("#formulario input, #formulario select, #formulario textarea").attr("disabled", true);
            $(".ncCheck").attr("disabled", false);
        }, 300);
    }
</script>
<?php 
if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

$habil = 'S';



if( count($rsPO) < 1 ): ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td>
				<center>
					<p><font>A Nota de Crédito foi enviada para o FNDE.</font></p>
				</center>
			</td>
		</tr>
	</table>	
<?php else: ?>
	<form id="formulario" name="formulario" method="post" action="elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=enviarNCfnde">	
		<input type="hidden" name="requisicao" value="" />
		<input type="hidden" name="evento_contabil" value="300300" />
		<input type="hidden" name="especie" value="3" />
		<input type="hidden" name="sistema" value="2" />
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">

        <?php if (verificaEfetivacaoNCSigef()) : ?>
            <tr>
                <td class="subtituloDireita">Usuário do SIGEF</td>
                <td>
                    <input type="text" name="sigefusername" id="sigefusername" value="" class="obrigatorio normal" size="25" />
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Senha do SIGEF</td>
                <td>
                    <?php  $senha = md5_decrypt_senha($db->pegaUm("select ususenha from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'"), ''); ?>
                    <input type="password" name="sigefpassword" id="sigefpassword" value="" class="obrigatorio normal" size="25" />
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" value="Verificar efetivação da NC SIGEF" class="ncCheck">
                </td>
            </tr>
        <? else: ?>

			<?php if($_SESSION['error']['ws']['sigef']['nc']): ?>
            <tr>
                <td colspan="2">
                    <div class="alert alert-danger col-md-8 col-md-offset-2">
                        <p><span style="font-weight:bold">Retorno do <span style="text-decoration:underline">SIGEF</span>:</span></p>
                        <p style="margin-left:12px;padding-left:5px;border-left:2px solid red"><?php echo $_SESSION['error']['ws']['sigef']['nc']; ?></p>
                    </div>
                </td>
            </tr>
			<?php endif; ?>
			<tr>
				<td class="subtituloDireita">Usuário do SIGEF</td>
				<td>
					<input type="text" name="sigefusername" value="<?php echo $_SESSION['usucpf']; ?>" class="obrigatorio normal" size="25" />
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Senha do SIGEF</td>
				<td>
					<?php  $senha = md5_decrypt_senha($db->pegaUm("select ususenha from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'"), ''); ?>
					<input type="password" name="sigefpassword" value="" class="obrigatorio normal" size="25" />
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			
			<tr>
				<td class="subtituloDireita">Número de Transferência do Termo</td>
				<td><?php echo campo_texto('tcpnumtransfsiafi', 'S', $habil, '', 45, '', '', ''); ?></td>
			</tr>
			<tr>
				<td class="subtituloDireita">Processo</td>
				<td><?php echo campo_texto('tcpnumprocessofnde', 'S', $habil, '', 20, 17, '', ''); ?></td>
			</tr>
			<tr>
				<td class="subtituloDireita">Cód. Unidade Orçamentária</td>
				<td><?php echo campo_texto('unicod', 'S', $habil, '', 20, 17, '', ''); ?></td>
			</tr>
			<tr>
				<td class="subtituloDireita">Cód. Unidade Gestora Emitente</td>
				<td><?php echo campo_texto('ungcodemitente', 'S', $habil, '', 20, 17, '', ''); ?></td>
			</tr>
			<tr>
				<td class="subtituloDireita">Cód. Centro de Gestão Emitente</td>
				<td><?php echo campo_texto('gescodemitente', 'S', $habil, '', 20, 17, '', ''); ?></td>
			</tr>
			<tr>
				<td class="subtituloDireita">Programa</td>
				<td>
					<?php 
					$sql = "select distinct prgcodfnde as codigo, prgcodfnde as descricao from elabrev.dadosprogramasfnde where eventocontabil = '300300' order by prgcodfnde";
					$tcpprogramafnde = $tcpprogramafnde ? $tcpprogramafnde : 'C7';
					$db->monta_combo('tcpprogramafnde', $sql, 'S', '', '', '', '', '', 'S', '', '');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Observação</td>
				<td id="td_combo_obs">
					<?php 
					$sql = "select distinct obscod as codigo, obscod as descricao from elabrev.dadosprogramasfnde where eventocontabil = '300300' and prgcodfnde = '{$tcpprogramafnde}' order by obscod";
					$tcpobsfnde = $tcpobsfnde ? $tcpobsfnde : '';
					$db->monta_combo('tcpobsfnde', $sql, 'S', '', '', '', '', '', 'S', '', '');
					?>
				</td>
			</tr>			
			<tr>
				<td class="subtituloDireita">Complemento da Observação</td>
				<td>
					<?php //echo campo_texto('tcpobscomplemento', 'S', $habil, '', 60, 140, '', ''); ?>
					<?php echo campo_textarea( 'tcpobscomplemento', 'S', $habil, '', 80, 4, 240, '', '', '', '', '',$tcpobscomplemento);?>
				</td>
			</tr>			
			<tr>
				<td class="subtituloDireita">Previsão(ões) Orçamentária(s)</td>
				<td>
                    <?php if($rsPO): ?>
                        <?php foreach($rsPO as $po): ?>
                            <?php
                            $sql = "SELECT DISTINCT
                                        pro.proid,
                                        ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
                                        substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
                                        substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
                                        pro.ptrid,
                                        a.acacod,
                                        pro.pliid,
                                        case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
                                        pro.ndpid,
                                        to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
                                        coalesce(pro.provalor, 0) as valor,
                                        crdmesliberacao,
                                        crdmesexecucao,
                                        pro.proid,
                                        pro.proanoreferencia,
                                        pro.prgidfnde,
                                        pro.esfid,
                                        pro.espid,
                                        pro.prgfonterecurso
                                    FROM monitora.previsaoorcamentaria pro
                                    LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
                                    LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
                                    LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
                                    LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
                                    LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
                                    LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
                                    LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
                                    WHERE pro.prostatus = 'A'
                                    AND pro.proid = ".$po['proid']."";
                            //ver($sql, d);
                            $rsPrevisao = $db->pegaLinha($sql);

                            if ($rsPrevisao) :
                            ?>

                                <table border="0">
                                    <tr id="tr_titulo">
                                        <td class="subtituloDireita"><center><b>Ano</b></center></td>
                                        <td class="subtituloDireita"><center><b>Ação</b></center></td>
                                        <td class="subtituloDireita"><center><b>Programa de Trabalho</b></center></td>
                                        <td class="subtituloDireita"><center><b>Plano Interno</b></center></td>
                                        <td class="subtituloDireita"><center><b>Descrição da Ação Constante da LOA</b></center></td>
                                        <td class="subtituloDireita"><center><b>Nat.da Despesa</b></center></td>
                                        <td class="subtituloDireita"><center><b>Valor</b></center></td>
                                        <td class="subtituloDireita"><center><b>Mês da Liberação</b></center></td>
                                        <td class="subtituloDireita"><center><b>Prazo para o cumprimento do objeto (meses)</b></center></td>
                                    </tr>
                                    <tr>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['proanoreferencia']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['acacod']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['ptrid_descricao']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['pliid_descricao']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['acatitulo']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['ndp_descricao']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['provalor']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['crdmesliberacao']; ?></td>
                                        <td class="subtituloDireita"><?php echo $rsPrevisao['crdmesexecucao']; ?></td>
                                    </tr>
                                </table>


                                    <?php
                                        $sqlPi = "select
                                                    prgid as codigo,
                                                    prgcodfnde || ' - ' || plicod || ' - ' || gescod || ' - ' ||
                                                    tpddoccod || ' - ' || obscod || ' - ' ||  eventocontabil as descricao
                                                from elabrev.dadosprogramasfnde
                                                where plicod = '{$po['plicod']}'
                                                and eventocontabil = '300300'
                                                order by prgcodfnde, gescod, obscod";
                                        //ver($sqlPi);

                                        $sqlEspecie = "select espid as codigo, espdsc as descricao from elabrev.tipoespecie order by espid";
                                        $sqlEsfera = "select esfid as codigo, esfdsc as descricao from elabrev.tipoesfera order by esfid";

                                        $sqlFonteRecurso = "select
                                                            frecodfonte as codigo,
                                                            frecodfonte || ' - ' || fredscfonte as descricao
                                                        from elabrev.fonterecurso
                                                        order by frecodfonte";

                                        $prgid = $rsPrevisao['prgidfnde'];
                                        $esfid = $rsPrevisao['esfid'] ? $rsPrevisao['esfid'] : 1;
                                        $espid = $rsPrevisao['espid'] ? $rsPrevisao['espid'] : 3;
                                        $prgfonterecurso = $rsPrevisao['prgfonterecurso'];
                                        $titleComboCelula = "Código Programa FNDE - Plano Interno - Centro Gestão - Tipo Documento - Observação - Evento Contábil";

                                        echo '<table width="100%">';
                                        echo '<tr>';
                                        echo '<td><input name="chekCel[]" value="'.$po['proid'].'" type="checkbox" checked/></td>';
                                        echo '<td class="subtituloDireita" width="110">Célula Orçamentaria</td>';
                                        echo '<td>';
                                        $_programa = isset($_REQUEST['prgid'][$po['proid']]) ? $_REQUEST['prgid'][$po['proid']] : $prgid;
                                        $db->monta_combo('prgid['.$po['proid'].']', $sqlPi, 'S', 'Selecione...', '', '', '', '', '', '', '', $_programa).'&nbsp;&nbsp;<img align="absmiddle" src="../imagens/ajuda.gif" alt="'.$titleComboCelula.'" title="'.$titleComboCelula.'" />';
                                        echo '</td>';
                                        echo '<td class="subtituloDireita" width="110">Espécie</td>';
                                        echo '<td>';
                                        $db->monta_combo('espid['.$po['proid'].']', $sqlEspecie, 'S', '', '', '', '', '', '', '', '', $espid).'';
                                        echo '</td>';
                                        echo '<td class="subtituloDireita" width="110">Esfera</td>';
                                        echo '<td>';
                                        $db->monta_combo('esfid['.$po['proid'].']', $sqlEsfera, 'S', '', '', '', '', '', '', '', '', $esfid).'';
                                        echo '</td>';
                                        echo '<td class="subtituloDireita" width="110">Cód. Fonte do Recurso</td>';

        // 								echo '<td>'.campo_texto('prgfonterecurso['.$po['proid'].']', 'S', 'S', '', 20, 10, '', '', '', '', '', '', '', $prgfonterecurso).'</td>';
                                        echo '<td>';
                                        $valor = isset($_REQUEST['prgfonterecurso'][$po['proid']]) ? $_REQUEST['prgfonterecurso'][$po['proid']] : null;
                                        $db->monta_combo('prgfonterecurso['.$po['proid'].']', $sqlFonteRecurso, 'S', 'Selecione...', '', '', '', 400, '', '', '', $valor).'';
                                        echo '</td>';


                                        echo '</tr>';
                                        echo '<tr><td colspan="9">&nbsp;</td></tr>';
                                        echo '</table>';

                                    endif;
                                ?>
                            <?php endforeach; ?>
                        <?php else: ?>
                            <?php echo '<script> desabilitaEnvioNc(); </script>'; ?>
                        <?php endif; ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">&nbsp;</td>
				<td class="subtituloEsquerda">
					<input type="button" id="idBtnEnviar" value="Enviar para FNDE" class="enviarWS"/>
				</td>
			</tr>

        <? endif; ?>
		</table>
	</form>
<?php endif; ?>

<script type="text/javascript">
    setTimeout(function(){

        if ($("select[name='tcpprogramafnde']").val()) {
            $.ajax({
                url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=enviarNCfnde',
                type	: 'post',
                data	: 'requisicao=atualizaObs&prgcodfnde='+$("select[name='tcpprogramafnde']").val(),
                success	: function(e){
                    $('#td_combo_obs').html(e);
                }
            });
        }

    }, 200);
</script>