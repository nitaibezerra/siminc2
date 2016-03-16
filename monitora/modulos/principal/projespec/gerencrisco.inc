<?php
	/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Gilberto Arruda Cerqueira Xavier
	Programador: Fabrício Mendonça(fabriciomendonca@gmail.com)
	Módulo:gerencrisco.inc
	Finalidade: permitir o controle de cadastro de riscos de um projeto especial
	*/

	/*
	 * 
	 */
	$pjeid = $_SESSION[ 'pjeid' ];
	$usucpf = $_SESSION[ 'usucpf' ];
	$rpeid = $_REQUEST[ 'rpeid' ];
	$usunome = $_SESSION[ 'usunome' ];
	
	$modulo = $_REQUEST['modulo'];

	
	if( $db->testa_responsavel_projespec( $_SESSION[ 'pjeid' ] ) )
	{
		$coordpje = true;
	}
	
	// verifica se é digitador
	if( $db->testa_digitador( $_SESSION[ 'pjeid' ], 'E' ) )
	{
		$digit = true;
	}
	
	// verifica se é super-usuário
	if( $db->testa_superuser() )
	{
		$coordpje = true;
		$_SESSION[ 'coordpje' ] = true;
		$digit = true;
	}
	
	if( $_REQUEST[ 'act' ] == 'alterar' )
	{
		$sql = "select ptoid, rpedsc, rpedescricao, rpecusto, rpeprobabilidade, rpegravidade, rpeevolucao_tempo, rprid, rperesposta, rpemonitoramento from monitora.risco_projeto where rpeid=".$rpeid;
		$linha = $db->pegaLinha( $sql );
		foreach($linha as $k=>$v) ${$k}=$v;
	}
	
	if( $_REQUEST[ 'act2' ] == "incluir" )
	{
		if (!$_REQUEST[ 'rpecusto' ]) $_REQUEST[ 'rpecusto' ]=0;
		$values = " (".
				  $_REQUEST[ 'ptoid' ] . ", " .				  
				  "'" . $_REQUEST[ 'rpedsc' ] . "', " .
				  "'" . $_REQUEST[ 'rpedescricao' ] . "', " .
				  $_REQUEST[ 'rpecusto' ] . ", ".
				  "'" . $_REQUEST[ 'rpeprobabilidade' ] . "', " .
				  "'" . $_REQUEST[ 'rpegravidade' ] . "', " .
				  "'" . $_REQUEST[ 'rpeevolucao_tempo' ] . "', " .
				  $_REQUEST[ 'rprid' ] . ", ".
				  "'" . $_REQUEST[ 'rperesposta' ] . "'," . 
				  "'" . $_REQUEST[ 'rpemonitoramento' ] . "'" .
				  ")";
		$sql = "insert into monitora.risco_projeto(ptoid, rpedsc, rpedescricao, rpecusto, rpeprobabilidade, rpegravidade, rpeevolucao_tempo, rprid, rperesposta,rpemonitoramento) values".$values;
		//dbg($sql,1);
		if( $db->executar( $sql ) )
		{
			$db->commit();
			$db->sucesso( $modulo );
		}
	}
	else if( $_REQUEST[ 'act2' ] == "alterar" && $rpeid )
	{
		$enviaEmail = false;
		if (!$_REQUEST[ 'rpecusto' ]) $_REQUEST[ 'rpecusto' ]=0;
		$set = 	  "ptoid = " . $_REQUEST[ 'ptoid' ] . ", " .				  
				  "rpedsc = '" . $_REQUEST[ 'rpedsc' ] . "', " .
				  "rpedescricao = '" . $_REQUEST[ 'rpedescricao' ] . "', " .
				  "rpecusto = " . $_REQUEST[ 'rpecusto' ] . ", ".
				  "rpeprobabilidade = '" . $_REQUEST[ 'rpeprobabilidade' ] . "', " .
				  "rpegravidade = '" . $_REQUEST[ 'rpegravidade' ] . "', " .
				  "rpeevolucao_tempo = '" . $_REQUEST[ 'rpeevolucao_tempo' ] . "', " .
				  "rprid = " . $_REQUEST[ 'rprid' ] . ", ".
				  "rperesposta = '" . $_REQUEST[ 'rperesposta' ] . "',".
				  "rpemonitoramento = '" . $_REQUEST[ 'rpemonitoramento' ] . "'";
		if( $_REQUEST[ 'rpeocorrencia' ] == 'O' )
		{
			$enviaEmail = true;
			$set .= ", rpeocorrencia = 'O', rpejustificativa_ocorrencia = '" . $_REQUEST[ 'rpejustificativa_ocorrencia' ] . "'";
		}
		
		$sql = "update monitora.risco_projeto set " . $set . " where rpeid=".$_REQUEST[ 'rpeid' ];
		//dbg($sql,1);
		
		if( $db->executar( $sql ) )
		{			
			if( $enviaEmail )
			{
				$sql = "select pjecod || ' - ' || pjedsc as dsc from monitora.projetoespecial where pjeid=".$pjeid;
				$nomeProjeto = $db->pegaUm( $sql );
				$sql = "select distinct pfl.pflcod,pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.ususexo, usu.usucpf as membro, usu.usuemail, usu.usunome || ' ('|| ee.entnome ||')' as usuario,usu.usufoneddd||'-'||usu.usufonenum as fone from seguranca.perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = " . $pjeid . " and rpu.rpustatus='A' inner join seguranca.usuario usu on usu.usucpf=rpu.usucpf left join entidade.entidade ee on ee.entid = usu.entid where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pflcod";
				$rs = @$db->carregar( $sql );
				$assunto = "Sinalização de ocorrência de risco no projeto especial";
				foreach( $rs as $linha )
				{
					foreach( $linha as $k => $v ) ${$k} = $v;
					$sexo = $ususexo == 'F' ? "Prezada Sra. " : "Prezado Sr. ";
					$body = $sexo. str_to_upper($usuario). chr(13) . "Reportamos que " . $usunome . " sinalizou a ocorrência de risco no projeto especial " . $nomeProjeto . ".\n";
					$body .= "Abaixo descrição do risco:\n\n";
					$sql = "select ptodsc from monitora.planotrabalho where ptoid=".$_REQUEST[ 'ptoid' ];
					$ptodsc = $db->pegaUm( $sql );
					$body .= "Atividade:\n" . $ptodsc . "\n\n";
					$body .= "Título:\n" . $_REQUEST[ 'rpedsc' ] . "\n\n";
					$body .= "Descrição:\n" . $_REQUEST[ 'rpedescricao' ] . "\n\n";
					$body .= "Custo:\n" . $_REQUEST[ 'rpecusto' ] . "\n\n";
					switch( $_REQUEST[ 'rpeprobabilidade' ] )
					{
						case 'B':
							$probabilidade = "Baixa";
							break;
						case 'M':
							$probabilidade = "Média";
							break;
						case 'A':
							$probabilidade = "Alta";
							break;
					}
					$body .= "Probabilidade:\n". $probabilidade . "\n\n";
					switch( $_REQUEST[ 'rpegravidade' ] )
					{
						case 'B':
							$gravidade = "Baixa";
							break;
						case 'M':
							$gravidade = "Média";
							break;
						case 'A':
							$gravidade = "Alta";
							break;
					}
					$body .= "Gravidade:\n". $gravidade . "\n\n";
					switch( $_REQUEST[ 'rpeevolucao_tempo' ] )
					{
						case 'D':
							$evolucao = "Diminui";
							break;
						case 'C':
							$evolucao = "Constante";
							break;
						case 'A':
							$evolucao = "Aumenta";
							break;
					}
					$body .= "Evolução com o tempo:\n". $evolucao . "\n\n";
					$sql = "select rprdsc from public.resposta_risco where rprid=".$_REQUEST[ 'rprid' ];
					$rprdsc = $db->pegaUm( $sql );
					$body .= "Tipo da resposta:\n". $rprdsc . "\n\n";
					$body .= "Resposta:\n" . $_REQUEST[ 'rperesposta' ] . "\n\n";
					$body .= "Justificativa da ocorrência:\n" . $_REQUEST[ 'rpejustificativa_ocorrencia' ] . "\n\n";
					email( $usuario, $usuemail, $assunto, $body );
				}
			}
			$db->commit();
			$db->sucesso( $modulo );
		}
	}
		include APPRAIZ."includes/cabecalho.inc";
			

	
	$db->cria_aba($abacod_tela,$url,'');
	monta_titulo($titulo_modulo,'');
?>

<form method="POST"  name="formulario" id="formulario">
<input type="hidden" name="act" value="" />
<input type="hidden" name="act2" value="" />
<input type="hidden" name="rpeid" value="<?=$rpeid;?>" />
<? if( $_REQUEST[ 'act' ] == "incluir" || $_REQUEST[ 'act' ] == "alterar" ): ?>
<table align="center" width="90%" cellpadding="3" cellspacing="1" border="0" class="tabela" bgcolor="#f5f5f5" >
	<tr>
    	<td align='right' class="SubTituloDireita" colspan="2" style="height:1px;padding:0px;"></td>
	</tr>	
		<tr>
			<td class="SubTituloDireita">Atividade:</td>
			<td>
			<?
				$sql = "select ptoid as codigo, ptodsc as descricao from monitora.planotrabalho where ptostatus='A' and ptosnaprovado='t' and pjeid=".$pjeid;
				$db->monta_combo( "ptoid", $sql, 'S', "Selecione a atividade",'','','',400);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Título:</td>
			<td>
				<?=campo_texto('rpedsc','S', 'S','',77, 100,'','','','Entre com o título do risco.');?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição:</td>
			<td><?=campo_textarea( 'rpedescricao', 'S', 'S', '', 80, 3, '', '', '', 'Entre com a descrição do risco.' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Custo:</td>
			<td><?=campo_texto('rpecusto','S', 'S','',14, 12,'############','','','Entre com o título do risco.');?>(valores em reais inteiros)</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Probabilidade:</td>
			<td>
			<?
				$opcoes = array
				(
					"Baixa" => array
					(
							"valor" => "B",
							"id"    => "probBaixa"	
					),
					"Média" => array
					(
							"valor" => "M",
							"id"    => "probMedia"	
					),
					"Alta" => array
					(
							"valor" => "A",
							"id"    => "probAlta"	
					)
				);
				campo_radio( 'rpeprobabilidade', $opcoes, 'h' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Gravidade:</td>
			<td>
			<?
				$opcoes = array
				(
					"Baixa" => array
					(
							"valor" => "B",
							"id"    => "gravBaixa"	
					),
					"Média" => array
					(
							"valor" => "M",
							"id"    => "gravMedia"	
					),
					"Alta" => array
					(
							"valor" => "A",
							"id"    => "gravAlta"	
					)
				);
				campo_radio( 'rpegravidade', $opcoes, 'h' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Evolução com o tempo:</td>
			<td>
			<?
				$opcoes = array
				(
					"Diminui" => array
					(
							"valor"   => "D",
							"id"      => "evoDiminui"
					),
					"Constante" => array
					(
							"valor"   => "C",
							"id"      => "evoConstante"	
					),
					"Agrava" => array
					(
							"valor"   => "A",
							"id"      => "evoAgrava"	
					)
				);
				campo_radio( 'rpeevolucao_tempo', $opcoes, 'h' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Monitoramento:</td>
			<td>
			<?
				$opcoes = array
				(
					"Semanal" => array
					(
							"valor"   => "S",
							"id"      => "S"
					),
					"Quinzenal" => array
					(
							"valor"   => "Q",
							"id"      => "Q"	
					),
					"Mensal" => array
					(
							"valor"   => "M",
							"id"      => "M"	
					),
					"Trimestral" => array
					(
							"valor"   => "T",
							"id"      => "T"	
					)	,				
					"Semestral" => array
					(
							"valor"   => "Z",
							"id"      => "Z"	
					)					
				);
				campo_radio( 'rpemonitoramento', $opcoes, 'h' );
			?>			
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita">Tipo resposta:</td>
			<td>
			<?
				$sql = "select rprid as codigo, rprdsc as descricao from public.resposta_risco where rprstatus='A'";
				$db->monta_combo( "rprid", $sql, 'S', "Selecione o tipo de resposta",'','','',400);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Resposta:</td>
			<td><?=campo_textarea( 'rperesposta', 'S', 'S', '', 80, 3, '', '', '', 'Entre com a resposta.' ); ?></td>
		</tr>
		<? if( $_REQUEST[ 'act' ] == 'incluir' ) : ?>
		<tr bgcolor="#CCCCCC">
			<td></td>
			<td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"><input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
		</tr>
		<? else : ?>
		<tr id="ocorrencia">
			<td class="SubTituloDireita"><div style="display:none;">Justificativa:</div></td>
			<td>
				<div style="display:none;"><?=campo_textarea( 'rpejustificativa_ocorrencia', 'S', 'S', '', 80, 3, '', '', '', 'Entre com a justificativa da ocorrência.' ); ?></div>
				<input type="hidden" name="rpeocorrencia" value="P" />
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
			<td></td>
			<td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">&nbsp;&nbsp;<input type="button" name="btsinalizar" id="btsinalizar" value="Sinalizar ocorrência" onclick="sinalizar_ocorrencia()" class="botao">&nbsp;&nbsp;<input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
		</tr>
		<? endif ?>
</table>
<? else: //Monta a listagem dos riscos por projeto especial ?>
<table width="95%" align='center' border="0" cellspacing="0" cellpadding="0" class="listagem">
		<tr>
		      <td class="title" align="center"><strong>Ação</strong></td>
		      <td class="title" style="padding:3px;"><strong>Risco</strong></td>
		</tr>
		<?
		$sql = "select rpeid, rpedsc from monitora.risco_projeto r inner join monitora.planotrabalho p using(ptoid) where p.pjeid=".$pjeid;
		$rs = $db->carregar( $sql );
		$i = 0;
		if( $rs ):
			// Cria as linhas com os registros encontrados na query
			foreach( $rs as $linha ):
				foreach($linha as $k=>$v) ${$k}=$v;
				if (fmod($i,2) == 0) $marcado = '#F7F7F7' ; else $marcado='';
		?>
				<tr bgcolor="<?=$marcado;?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado;?>';">
					<td width="60" align="center" style="padding:3px;"><img border="0" src="../imagens/alterar.gif" title="Editar o risco." onclick="alterar_risco('<?=$rpeid;?>')">&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir o risco." onclick="excluir_risco('<?=$rpeid;?>')"></td>
					<td style="padding:3px;"><?=$rpedsc;?></td>
				</tr>
		<?
				$i++;
			endforeach;
		else :
		?>
			<tr>
					<td width="60" align="center" style="padding:3px;"></td>
					<td style="padding:3px;">Nenhum risco cadastrado</td>
			</tr>
		<?
		endif;
		?>
		<tr>
			<td></td>
			<td><input type="button" value="Incluir novo risco" onclick="inclui_risco()"></td>
		</tr>
</table>
<? endif //Fim da verificação da variável $_REQUEST[ 'act' ] ?>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript">
	
	/**
	* Valida os campos obrigatórios do formulário
	* 
	* @param string acao - I para inclusão e A para alteração
	* @return void
	*/
	function validar_cadastro( acao )
	{
		formulario = document.formulario;
		if( !validaBranco( formulario.ptoid, "Atividade" ) ) return;
		
		if( !validaBranco( formulario.rpedsc, "Título" ) ) return;
		
		if( !validaBranco( formulario.rpedescricao, "Descrição" ) ) return;
		
		if( document.formulario.act.value == 'alterar' )
		{
			if( formulario.rpeocorrencia.value == 'O' )
				if( !validaBranco( formulario.rpejustificativa_ocorrencia, "Justificativa" ) ) return;
		}
		
		if( acao == "I" )
		{
			formulario.act2.value = "incluir";
		}
		else
		{
			formulario.act2.value = "alterar";
		}
		formulario.submit();
	}
	
	/**
	* Submete a variável act via post para montar o formulario de inclusao
	* 
	* @return void
	*/
	function inclui_risco()
	{
		document.formulario.rpeid.value = '';
		document.formulario.act.value = 'incluir';
		document.formulario.submit();
	}
	
	function alterar_risco( rpeid )
	{
		if( rpeid )
		{
			document.formulario.rpeid.value = rpeid;
			document.formulario.act.value = 'alterar';
			document.formulario.submit();
		}
	}
	
	function sinalizar_ocorrencia()
	{
		linha = document.getElementById( 'ocorrencia' );
		linha.getElementsByTagName( 'div' )[ 0 ].style.display = 'block';
		linha.getElementsByTagName( 'div' )[ 1 ].style.display = 'block';
		document.getElementById( 'btsinalizar' ).style.display = 'none';
		document.formulario.rpeocorrencia.value = 'O';
	}
	
</script>