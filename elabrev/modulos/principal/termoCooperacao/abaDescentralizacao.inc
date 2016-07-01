<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
$(document).ready(function(){
	if( $('[name="tcptipoemenda"]:checked').val() == 'S' ){
		$('.tr_emenda').css('display', '');
	} else {
		$('.tr_emenda').css('display', 'none');
	}
	
	$('[name="tcptipoemenda"]').click(function(){
		if( $(this).val() == 'S' ){
			$('.tr_emenda').css('display', '');
		} else {
			$('.tr_emenda').css('display', 'none');
		}
	});
});
</script>
<?php

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

verificaCodigoTermoSessao();
	
$sql = "SELECT  
			ungcodproponente,
			ungcodconcedente,
			tcpdscobjetoidentificacao,
			tcpobjetivoobjeto,
			tcpobjetojustificativa,
			ungcodgestaorecebedora as ungcodrec,
			ungcodpoliticafnde,
			tcptipoemenda
		FROM 
			monitora.termocooperacao
		WHERE 
			tcpid = ".$_SESSION['elabrev']['tcpid'];

$dadoObjetivo = !empty($_SESSION['elabrev']['tcpid']) ? $db->pegaLinha($sql) : array();
//ver($dadoObjetivo, d);

$sqlEmenda = "select 
			count(e.emeid)
		from emenda.emenda e
			inner join emenda.v_emendadetalheentidade ve on ve.emeid = e.emeid
			inner join emenda.entidadebeneficiada en on en.enbid = ve.entid
		where
			en.enbcnpj in (select u.ungcnpj from public.unidadegestora u where ungcod = '{$dadoObjetivo['ungcodproponente']}')
		    and e.etoid = 2
		    and ve.ededisponivelpta = 'S'
		    and e.emeano = '{$_SESSION['exercicio']}'
		    and ve.edestatus = 'A'";

$boExisteEmenda = $db->pegaUm($sqlEmenda);
?>
<form method="post" name="formulario" id="formulario">

	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	
	<input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Objeto e Justificativa da Descentralização do Crédito</b></center>
			</td>
		</tr>
	</table>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<?if( (int)$boExisteEmenda > 0 ){ ?>
		<tr>
			<td class="SubTituloDireita" width="15%">É do tipo Emenda?</td>
			<td>
				&nbsp;<input type="radio" name="tcptipoemenda" id="tcptipoemenda_s" value="S" <?=($dadoObjetivo['tcptipoemenda'] == 'S' ? 'checked="checked"' : ''); ?> >Sim&nbsp;
					  <input type="radio" name="tcptipoemenda" id="tcptipoemenda_n" value="N" <?=($dadoObjetivo['tcptipoemenda'] != 'S' ? 'checked="checked"' : ''); ?> >Não
			</td>
		</tr>
		<tr class="tr_emenda">
			<td class="SubTituloDireita" width="15%">Emenda:</td>
			<td>
				<?
				$sql = "select 
									e.emeid as codigo,
								    e.emecod as descricao
								from emenda.emenda e
									inner join emenda.v_emendadetalheentidade ve on ve.emeid = e.emeid
									inner join emenda.entidadebeneficiada en on en.enbid = ve.entid
								where
									en.enbcnpj in (select u.ungcnpj from public.unidadegestora u where ungcod = '{$_SESSION['elabrev']['ungcodlista']}')
								    and e.etoid = 2
								    and ve.ededisponivelpta = 'S'
								    and e.emeano = '{$_SESSION['exercicio']}'
								    and ve.edestatus = 'A'";
				$emeid = $db->pegaUm("select emeid from emenda.emendatermocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}");
				
				$db->monta_combo( "emeid", $sql, 'S', 'Selecione...', "", '', '', '265','','emeid');
				?>
			</td>
		</tr>
		<?} ?>
		<tr>
			<td class="SubTituloDireita" width="15%">Identificação (Título / Objeto da despesa):</td>
			<td>
				<?=campo_textarea( 'tcpdscobjetoidentificacao', 'S', $habilita, '', 150, 3, 70, '', '', '', '', '', $dadoObjetivo['tcpdscobjetoidentificacao']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Objetivo: </td>
			<td>
				<table>
					<tr>
						<td>
							<?=campo_textarea( 'tcpobjetivoobjeto', 'S', $habilita, '', 150, 5, 489, '', '', OBJETIVO_HINT, '', '', $dadoObjetivo['tcpobjetivoobjeto']) ?>
						</td>
						<td>
							<img src="../imagens/info.gif" height="15" alt="<?php echo OBJETIVO_HINT ?>" title="<?php echo OBJETIVO_HINT ?>" border="0" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">UG/Gestão Repassadora: </td>
			<td>
			<?php
				if($dadoObjetivo['ungcodconcedente']){
					$sql = "SELECT
								ungcod as codigo,
								ungcod||' / '||ungdsc as descricao
							FROM
								public.unidadegestora
							WHERE
								ungstatus = 'A' and ungcod = '".$dadoObjetivo['ungcodconcedente']."'
							ORDER BY
							2";
					$dado = $db->pegaLinha($sql);
					echo $dado['descricao'];
				}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">UG/Gestão Recebedora: </td>
			<td>
			<?php 
				if($dadoObjetivo['ungcodproponente']){
					$sql = "
						SELECT	ungcod as codigo,
								ungcod||' / '||ungdsc as descricao
						FROM public.unidadegestora
						WHERE ungstatus = 'A' and ungcod = '".$dadoObjetivo['ungcodproponente']."'
						ORDER BY 2
					";
					$dados = $db->pegaLinha($sql);
					echo $dados['descricao'];
				}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">
				Justificativa (Motivação / Clientela / Cronograma físico):
			</td>
			<td>
				<table>
					<tr>
						<td>
							<?=campo_textarea( 'tcpobjetojustificativa', 'S', $habilita, '', 150, 4, 349, '', '', JUSTIFICATIVA_HINT, '', '', $dadoObjetivo['tcpobjetojustificativa']);?>
						</td>
						<td>
							<img src="../imagens/info.gif" height="15" alt="<?php echo JUSTIFICATIVA_HINT ?>" title="<?php echo JUSTIFICATIVA_HINT ?>" border="0" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Relações entre as Partes</td>
			<td>				
				<?php if($dadoObjetivo['ungcodconcedente'] == UG_FNDE && in_array($dadoObjetivo['ungcodpoliticafnde'], array(UG_SECADI, UG_SETEC, UG_SEB))): ?>
					<p>
					I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei no 8.666, de 21 de junho de 1993, no que couber, Decreto no  93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011 e Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012.
					</p>
					<p>
					II - constituem obrigações da CONCEDENTE:<br>
					a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br>
					</p>
					<p>
					III - constituem obrigações do GESTOR DO PROGRAMA:<br/> 
					a) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br>
					b) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br>
					c) analisar o relatório de cumprimento do objeto do presente Termo;
					</p>
					<p>
					IV - constituem obrigações da PROPONENTE:<br>
					a) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br>
					b) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br>
					c) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br>
					d) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem o Programa;<br>
					e) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br>
					f) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br>
					g) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br>
					h) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.
					</p>
				<?php else: ?>
					<p>
					I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei no 8.666, de 21 de junho de 1993, no que couber, Decreto no  93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011 e Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012.
					</p>
					<p>
					II - constituem obrigações da CONCEDENTE:<br>
					a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br>
					b) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br>
					c) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br>
					d) analisar o relatório de cumprimento do objeto do presente Termo;
					</p>
					<p>
					III - constituem obrigações da PROPONENTE:<br>
					a) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br>
					b) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br>
					c) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br>
					d) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem o Programa;<br>
					e) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br>
					f) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br>
					g) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br>
					h) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.
					</p>
				<?php endif; ?>
			</td>
		</tr>
	</table>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if($habilitaSalvar == 'S'): ?>
					<input type="button" id="concedente" value="Gravar e Voltar" class="navegarSalvar"/> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>
					<input type="button" id="previsao" value="Gravar e Continuar" class="navegarSalvar"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>