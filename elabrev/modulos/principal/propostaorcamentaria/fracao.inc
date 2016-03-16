<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: fracao.inc
		Finalidade: distribuir o montante de um proposta de uma matriz para as unidades na tabela fracao
	*/

	// FUNÇÔES DA PÁGINA
	
		/**
		 * Insere um registro na tabela elabrev.fracao e retorna
		 * seu identificador em caso de sucesso ou 0 em caso de falha.
		 *
		 * @param integer $mtmid
		 * @param string $unicod
		 * @param string $unitpocod
		 * @param float $fravalor
		 * @return integer
		 */
		function insere_fracao( $mtmid, $unicod, $unitpocod, $fravalor )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$fravalor = (float) $fravalor;
			$fonteMatriz=(integer)$db->pegaUm( "select fr.foncod from public.fonterecurso fr inner join elabrev.fonterecursomatriz frm ON frm.foncod = fr.foncod inner join elabrev.matriz m ON m.mtrid = frm.mtrid inner join elabrev.montantematriz mm ON mm.mtrid = m.mtrid where mm.mtmid = ".$mtmid."");
			$vlMontante=$db->pegaUm( "select mtmvlrlimite from elabrev.montantematriz where mtmid = ".$mtmid."");
			
			if ($fravalor != 0)
			$vlLimite = ($fravalor * $vlMontante)/100;
			else
			$vlLimite = 0;
						
			$sql = "insert into elabrev.fracao ( mtmid, unicod, unitpocod, fravalor ) values( " . $mtmid . ", '" . $unicod . "', '" . $unitpocod . "', " . $fravalor . " )";
			$sql2 = "insert into elabrev.limitefonte ( foncod, unicod, unitpocod, mtmid, lmfvalor ) values( " . $fonteMatriz . " , " . $unicod . ", '" . $unitpocod . "', '" . $mtmid . "', " . $fravalor . " )";

			if ( !$db->executar( $sql ))
			{
				return 0;
			}
			$db->executar( $sql2 );
			return $db->pegaUm( "select currval( 'elabrev.fracao_fraid_seq'::regclass ) as mtmid" );
		}
		
		/**
		 * Captura dados das unidades e suas respectivas frações
		 * de um montante ordenado pelo cógido e nome da unidade.
		 *
		 * @param integer $mtmid
		 * @return string[]
		 */
		function pegar_unidades_fracoes( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql = "select unidade.unicod, unidade.unitpocod, unidade.unidsc, fracao.fravalor, montantematriz.mtmid from unidade inner join elabrev.unidadematriz on ( unidade.unitpocod = unidadematriz.unitpocod and unidade.unicod = unidadematriz.unicod ) inner join elabrev.matriz on matriz.mtrid = unidadematriz.mtrid inner join elabrev.montantematriz on matriz.mtrid = montantematriz.mtrid left join elabrev.fracao on ( unidade.unitpocod = fracao.unitpocod and unidade.unicod = fracao.unicod and fracao.mtmid = montantematriz.mtmid ) where montantematriz.mtmid = " . $mtmid . " order by unidade.unicod, unidade.unidsc";
			$dados = $db->carregar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}
		
		/**
		 * Captura os dados de um montante
		 *
		 * @param integer $mtmid
		 * @return string[]
		 */
		function pegar_montante( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql = "select matriz.mtrdsc, matriz.mtrid, matriz.mtrmatriz, propostaorcamento.ppodsc, propostaorcamento.ppoano, montantematriz.mtmvlrlimite, montantematriz.mtmid from elabrev.montantematriz inner join elabrev.matriz on montantematriz.mtrid = matriz.mtrid inner join elabrev.propostaorcamento on propostaorcamento.ppoid = montantematriz.ppoid where montantematriz.mtmid = " . $mtmid;
			$dados = $db->recuperar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}

		/**
		 * Remove todas os registros da tabela elabrev.fracao que
		 * pertence ao montante indicado pelo parâmetro.
		 *
		 * @param integer $mtmid
		 * @return boolean
		 */
		function remove_fracoes_de_montante( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql_remover = "delete from elabrev.fracao where mtmid = " . $mtmid;
			$sql_remover2 = "delete from elabrev.limitefonte where mtmid = " . $mtmid;
			$db->executar( $sql_remover2 );
			return (boolean) $db->executar( $sql_remover );
		}
	
	// FIM FUNÇÔES DA PÁGINA
	
	// PRÉ-PROCESSAMENTO
	
		/**
		 * Quantidade de casas decimais que os percentuais poderão
		 * possuir. O máximo que o JavaScript suporta é 13.
		 * 
		 * @var integer
		 */
		define( 'PRECISAO_PERCENTUAL', 10 );
		
		/**
		 * Quantidade de casas decimais que os valores poderão
		 * possuir.
		 * 
		 * @var integer
		 */
		define( 'PRECISAO_VALOR', 0 );
	
		// VARIÁVEIS
		
			$mtmid = (integer) md5_decrypt( $_REQUEST['mtmid'], '' );
			$ppoid = $db->pegaUm( "select ppoid from elabrev.montantematriz where mtmid = " . $mtmid );
		
		// FIM VARIÁVEIS
		
		// MANIPULA SUBMISSÃO DE FORMULÁRIO
		
			// verifica se formulário foi submetido
			if ( array_key_exists( 'act', $_REQUEST ) )
			{
				// verifica se percentual não ultrapassa 100 %
				$soma_percentual = 0;
				foreach ( $_REQUEST['fracao'] as $fracao )
				{
					$soma_percentual += $fracao[2];
				}
				// caso ultrapasse nenhuma modificação no bando é realizada
				if ( $soma_percentual <= 100 )
				{
					// realiza requisições ao banco
					if ( $sucesso = remove_fracoes_de_montante( $mtmid ) )
					{
						// insere frações uma-a-uma
						// observar código html para entender os índices numéricos
						foreach ( $_REQUEST['fracao'] as $fracao )
						{
							$fravalor = (float) str_replace( ',', '.', $fracao[2] );
							if ( $fravalor > 0 )
							{
								if ( insere_fracao( $mtmid, $fracao[0], $fracao[1], $fravalor ) == 0 )
								{
									$sucesso = false;
									break;
								}
							}
						}
					}
					if ( $sucesso == true )
					{
						$db->commit();
					}
				}
			}
			
		// FIM MANIPULA SUBMISSÃO DE FORMULÁRIO
		
		$dados_montante = pegar_montante( $mtmid );
		$dados_unidade_fracao = pegar_unidades_fracoes( $mtmid );
		// somente as matrizes com o flag mtrmatriz verdadeiros podem ser manipuladas nessa página
		if ( count( $dados_unidade_fracao ) == 0 || $dados_montante['mtrmatriz'] != 't' )
		{
			eof();
		}
		$ppodsc = $dados_montante['ppodsc'];
		$ppoano = $dados_montante['ppoano'];
		
		// variáveis com valores globais refenrentes ao montante
		$montante_percentual = 100;
		$montante_valor = $dados_montante['mtmvlrlimite'];
		$distribuido_percentual = 0;
		$distribuido_valor = 0;
		$diferenca_percentual = 0;
		$diferenca_valor = 0;
		
		// altera os valores dos dados de acordo com os carregados do banco
		foreach ( $dados_unidade_fracao as $dado_fracao )
		{
			$distribuido_percentual += $dado_fracao['fravalor'];
		}
		$distribuido_valor = ( $distribuido_percentual / 100 ) * $montante_valor;
		$diferenca_percentual = $montante_percentual - $distribuido_percentual;
		$diferenca_valor = abs( $montante_valor - $distribuido_valor );
		
		// prepara dados gerais para serem exibidos nos campos do formulário
		$montante_percentual = formata_valor( $montante_percentual, 2, true );
		$montante_valor = formata_valor( $montante_valor, PRECISAO_VALOR );
		$distribuido_percentual = formata_valor( $distribuido_percentual, PRECISAO_PERCENTUAL, false );
		$distribuido_valor = formata_valor( $distribuido_valor, PRECISAO_VALOR );
		$diferenca_percentual = formata_valor( $diferenca_percentual, PRECISAO_PERCENTUAL, false );
		$diferenca_valor = formata_valor( $diferenca_valor, PRECISAO_VALOR );
		
		include APPRAIZ . "includes/cabecalho.inc";
		
	// FIM PRÉ-PROCESSAMENTO

?>
<br/>
<? monta_titulo( 'Dados da Proposta', '' ); ?>
<!-- FORMULÁRIO DE PROPOSTA ORÇAMENTÁRIA -->
<form action="" name="" method="post">
	<input type="hidden" name="modulo" value="principal/propostaorcamentaria/montante"/>
	<input type="hidden" name="acao" value="A"/>
	<input type="hidden" name="idproposta" value="<?= $ppoid ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" >Descrição:</td>
			<td><?= campo_texto( 'ppodsc', 'N', 'N', '', 130, 100, '', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >Exercício:</td>
			<td><?= campo_texto( 'ppoano','N', 'N', '', 6, 4, '', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >&nbsp;</td>
			<td style="margin:0;padding:0;">
				<input type="submit" name="alterar_proposta" value="Visualizar Proposta" />
			</td>
		</tr>
	</table>
</form>
<!-- FIM FORMULÁRIO DE PROPOSTA ORÇAMENTÁRIA -->
<? monta_titulo( 'Distribuição do Montante', $dados_montante['mtrdsc'] ); ?>
<!-- FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
<form action="" method="post" name="formulario">
	<input type="hidden" name="modulo" value="<?= $modulo ?>">
	<input type="hidden" name="mtmid" value="<?= $_REQUEST['mtmid'] ?>">
	<input type="hidden" name="act" value="1">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr bgcolor="#cecece">
			<td align="left" width="430"><b>Unidades</b></td>
			<td align="right" width="125">Percentual</td>
			<td align="right" width="125">Valor R$</td>
			<td><font color="#cecece"> + </font></td><!-- se tirar conteúdo desse <td> a tabela deforma no Microsoft Internet explorer -->
		</tr>
		<tr bgcolor="#dedede">
			<td align="right" width="430">Limite Global (A):</td>
			<td align="right" width="125"><?= campo_texto( 'montante_percentual', 'N', 'N', '', 19, 14, '', 'N', 'right', '', 0, '' ) ?></td>
			<td align="right" width="125"><?= campo_texto( 'montante_valor', 'N', 'N', '', 20, 14, '', 'N', 'right', '', 0, '' ) ?></td>
			<td>&nbsp;</td>
		</tr>
		<tr valign="top">
			<td colspan="4" style="padding:0;margin:0;height:200px">
				<div style="width: 100%; padding: 0; margin: 0; height: 200px; overflow: auto; position-y: absolute;">
					<table style="width:100%;padding:0;margin:0;" bgcolor="#f5f5f5" cellspacing="0" cellpadding="3" align="center">
						<? if ( count( $dados_unidade_fracao ) ) : ?>
							<?
								// necessário pois a variavel $montante_valor, neste ponto, está formatada para apresentação para o usuário
								$montate_valor_real = desformata_valor( $montante_valor );
							?>
							<? foreach ( $dados_unidade_fracao as $posicao => $dado_fracao ) : ?>
								<?php
									// os dados de cada fracao esta no campo array 'fracao' onde esse campo tambem é um array, onde:
									// indice 0 = codigo da unidade
									// indice 1 = tipo da unidade
									// indice 2 = percentual ( fravalor )
									// indice 3 = valor em relação ao total do montante ( dado insignificante para o sistema, utilizado apenas para mostrar ao usuário )
									// os índices 0 e 1 são utilizados junto com o mtmid para identificar a fração que está sendo editada
									$cor = $posicao % 2 == 0 ? 'f7f7f7' : 'ffffff' ;
									$nome_fracao_percentual = 'fracao[' . $posicao . '][2]';
									$nome_fracao_valor = 'fracao[' . $posicao . '][3]';
									// ações responsável por atualizar tela de acordo com as modificações do usuário
									$complemento = ' onkeyup="atualizar_percentual( this, ' . $posicao . ' );" onblur="verifica_formato_porcentagem( this, ' . $posicao . ' );MouseBlur(this);" tabindex="' . ( $posicao + 1 ) . '" ';
									// prepara variavel global de porcentagem para a funcao campo_texto()
									$fracao_percentual = str_replace( '.', ',', $dado_fracao['fravalor'] );
									if ( $fracao_percentual == '0' )
									{
										$fracao_percentual = '';
									}
									${$nome_fracao_percentual} = $fracao_percentual;
									// prepara variavel global de valor para a funcao campo_texto()
									$fracao_valor = formata_valor( ( $dado_fracao['fravalor'] / 100 ) * $montate_valor_real, PRECISAO_VALOR );
									if ( $fracao_valor == '0' )
									{
										$fracao_valor = '';
									}
									${$nome_fracao_valor} = $fracao_valor;
								?>
								<tr bgcolor="#<?= $cor ?>">
									<td align="right" width="20" style="color:#0066cc;" title="Código da Unidade"><?= $dado_fracao['unicod'] ?></td>
									<td align="left" width="395" title="Unidade Orçamentária"><?= $dado_fracao['unidsc'] ?></td>
									<td align="right" width="125"><?= campo_texto( $nome_fracao_percentual, 'N', 'S', '', 19, 14, '', 'N', 'right', '', 0, $complemento ) ?></td>
									<td align="right" width="125"><?= campo_texto( $nome_fracao_valor, 'N', 'N', '', 20, 14, '', 'N', 'right', '', 0, ' tabindex="0" onblur="MouseBlur(this);" ' ) ?></td>
									<td>
										<input type="hidden" name="fracao[<?= $posicao ?>][0]" value="<?= $dado_fracao['unicod'] ?>" />&nbsp;
										<input type="hidden" name="fracao[<?= $posicao ?>][1]" value="<?= $dado_fracao['unitpocod'] ?>" />&nbsp;
									</td>
								</tr>
							<? endforeach; ?>
						<? else : ?>
							<tr><td colspan="4">Não há unidades para esta coluna.</td></tr>
						<? endif; ?>
						</table>
				</div>
			</td>
		</tr>
		<tr bgcolor="#cecece">
			<td align="right" width="430">Limite Distribuido (B):</td>
			<td align="right" width="125"><?= campo_texto( 'distribuido_percentual', 'N', 'N', '', 19, 14, '', 'N', 'right', '', 0, '' ) ?></td>
			<td align="right" width="125"><?= campo_texto( 'distribuido_valor', 'N', 'N', '', 20, 14, '', 'N', 'right', '', 0, '' ) ?></td>
			<td>&nbsp;</td>
		</tr>
		<tr bgcolor="#dedede">
			<td align="right" width="430">Diferença (C) = (A-B):</td>
			<td align="right" width="125"><?= campo_texto( 'diferenca_percentual', 'N', 'N', '', 19, 14, '', 'N', 'right', '', 0, '' ) ?></td>
			<td align="right" width="125"><?= campo_texto( 'diferenca_valor', 'N', 'N', '', 20, 14, '', 'N', 'right', '', 0, '' ) ?></td>
			<td>&nbsp;</td>
		</tr>
		<tr bgcolor="#cccccc">
			<td colspan="4" style="text-align: right;">
				<input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="javascript:submeter_formulario();"/>
			</td>
		</tr>
	</table>
</form>
<!-- FIM FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
<script language="javascript">
	
	<!--
		
		/**
		 * Olhar documentação da constante php.
		 * 
		 * @var integer
		 */
		var PRECISAO_PERCENTUAL = <?= PRECISAO_PERCENTUAL ?>;
		
		/**
		 * Olhar documentação da constante php.
		 * 
		 * @var integer
		 */
		var PRECISAO_VALOR = <?= PRECISAO_VALOR ?>;
		
		/**
		 * Indica se o usuário alterou em algum momento os valores de
		 * distribuição. Caso o usuário altere o valore original e
		 * posteriormente retorne ao valor original esta variável será
		 * verdadeira.
		 * 
		 * @var boolean
		 */
		var houve_modificacao = false;
		
		/**
		 * Armazena todos os percentuais de cada unidade. A chave é não
		 * significativa. Lista utilizada para aumentar a performace e
		 * evitar longos laços nos campos do formulário.
		 * 
		 * @var float[]
		 */
		var percentuais = new Array();
		
		/**
		 * Armazena todos os valores de cada unidade. A chave é não
		 * significativa. Lista utilizada para aumentar a performace e
		 * evitar longos laços nos campos do formulário.
		 * 
		 * @var float[]
		 */
		var valores = new Array();
		
		// inicializa os dados de percentuais e valores de cada unidade
		<? foreach ( $dados_unidade_fracao as $posicao => $dados_fracao ) : ?>
			percentuais[<?= $posicao ?>] = <?= (float) $dados_fracao['fravalor'] ?>;
			valores[<?= $posicao ?>] = <?= (float) ( ( $dados_fracao['fravalor'] / 100 ) * desformata_valor( $montante_valor ) ) ?>;
		<? endforeach; ?>
		
		/**
		 * Porcentagem inicial que já foi distribuído do montante.
		 * 
		 * @var float
		 */
		var distribuido_percentual_inicial = <?= desformata_valor( $distribuido_percentual ) ?>;
		
		/**
		 * Porcentagem que já foi distribuído do montante.
		 * 
		 * @var float
		 */
		var distribuido_percentual = <?= desformata_valor( $distribuido_percentual ) ?>;
		
		/**
		 * Valor que já foi distribuído do montante.
		 * 
		 * @var float
		 */
		var distribuido_valor = <?= desformata_valor( $distribuido_valor ) ?>;
		
		/*
			não existe variáveis para os "campos":
			- montante_valor, pois esse valor é fixo e quando requisitado é capturado no input que armazena seu valor
			- diferenca_porcentagem, é relativo em relação ao montante_valor e distribuido_percentual
			- diferenca_valor, idem diferenca_porcentagem
		*/
		
		/**
		 * Atualiza valores dos campos em relação às porcentagens digitadas
		 * pelo usuário. Função deve ser chamada quando algum campo que
		 * contém as porcentagens é alterado.
		 * 
		 * @param object
		 * @param integer
		 * @return void
		 */
		function atualizar_percentual( input, indice )
		{
			houve_modificacao = true;
			
			// remove valores referentes ao input nas somas gerais
			distribuido_percentual -= parseFloat( percentuais[indice] );
			distribuido_valor -= parseFloat( valores[indice] );
			percentuais[indice] = 0;
			valores[indice] = 0;
			
			// atualiza valor do campo
			input.value = formata_percentual( input.value ); // atualiza o campo alterado
			
			// define valores referentes ao input nas somas gerais
			var elementos = document.forms['formulario'].elements; // array com os campos do formulário
			var montante = desformata_decimal( elementos['montante_valor'].value );
			percentuais[indice] = desformata_decimal( input.value );
			valores[indice] = ( percentuais[indice] / 100 ) * montante;
			distribuido_percentual += percentuais[indice];
			distribuido_valor += valores[indice];
			
			// atualiza campos do formulário
			elementos['fracao[' + indice + '][3]'].value = mascaraglobal( '###.###.###.###', valores[indice].toFixed( PRECISAO_VALOR ) );
			elementos['distribuido_percentual'].value = formata_decimal( distribuido_percentual.toFixed( PRECISAO_PERCENTUAL ).replace( '.', ',' ) );
			elementos['distribuido_valor'].value = mascaraglobal( '###.###.###.###', distribuido_valor.toFixed( PRECISAO_VALOR ).replace( '.', '' ) );
			var campo_diferenca_percentual = elementos['diferenca_percentual'];
			var campo_diferenca_valor = elementos['diferenca_valor'];
			// campos que mostram diferenças devem mostrar algum valor, mesmo que esse valor seja zero
			var percentual_diferenca = formata_decimal( Math.abs( 100 - distribuido_percentual ).toFixed( PRECISAO_PERCENTUAL ).replace( '.', ',' ) );
			if ( percentual_diferenca == '' )
			{
				percentual_diferenca = '0';
			}
			campo_diferenca_percentual.value = percentual_diferenca;
			campo_diferenca_valor.value = mascaraglobal( '###.###.###.###', ( montante - distribuido_valor ).toFixed( PRECISAO_VALOR ).replace( '.', '' ) );
			
			// atualiza estilo dos campos de diferenca
			// muda para o estilo 'disabled' caso a distribução ultrapasse 100 % )
			var estilo = distribuido_percentual > 100 ? 'erroInput' : 'disabled' ;
			campo_diferenca_percentual.className = estilo;
			campo_diferenca_valor.className = estilo;
		}
		
		/**
		 * Transforma uma string com vírgulas e pontos em um número real.
		 * 
		 * @param string
		 * @return float
		 */
		function desformata_decimal( valor )
		{
			if ( valor == null )
			{
				return 0.0;
			}
			valor = valor.replace( /\./g, '' );
			valor = valor.replace( ',', '.' );
			if ( valor[valor.length] == '.' )
			{
				valor += '0';
			}
			if ( valor.length == 0 )
			{
				return 0.0;
			}
			return parseFloat( valor );
		}
		
		/**
		 * Retira todos os caracteres não numéricos, exceto a vírgula. Caso
		 * exista mais de uma vírgula remove deixando apenas a última. Esta
		 * função não garante que o formato do percentual esteja correto. Deve
		 * ser utilizada durante a edição de um campo onde seu valor deve ser
		 * um decimal.
		 * 
		 * @param string
		 * @return string
		 */
		function formata_percentual( valor )
		{
			valor += '';
			var quantidade_virgulas = 0;
			var caracteres_invalidos = new Array();
			var auxiliar = 0;
			// conta quantidade de vírgulas
			auxiliar = valor.length;
			for ( var i = 0; i < auxiliar; i++ )
			{
				if ( valor.charAt( i ) == ',' )
				{
					quantidade_virgulas++;
				}
				else if ( isNaN( valor.charAt( i ) ) == true )
				{
					caracteres_invalidos[caracteres_invalidos.length] = valor.charAt( i );
				}
			}
			// retira as vírgulas ( deixa apenas uma )
			while ( quantidade_virgulas > 1 )
			{
				valor = valor.replace( ',', '' );
				quantidade_virgulas--;
			}
			// retira caracteres inválidos
			auxiliar = caracteres_invalidos.length;
			for ( i = 0; i < auxiliar; i++ )
			{
				valor = valor.replace( caracteres_invalidos[i], '' );
			}
			// retira os zeros à esquerda da vírgula caso a string seja maior que um dígito ( evita as situações '0,#' e '0' )
			if ( valor.indexOf( ',' ) != 0 )
			{
				while ( valor.charAt( 0 ) == '0' && valor.length > 1 )
				{
					valor = valor.substr( 1 );
				}
			}
			// se o primeiro caracter for vírgula o número '0' é adicionado
			if ( valor.charAt( 0 ) == ',' )
			{
				valor = '0' + valor;
			}
			return valor;
		}
		
		/**
		 * Realiza formatações em um texto que representa um número real.
		 * 
		 * @param string
		 * @return string
		 */
		function formata_decimal( valor )
		{
			valor += '';
			// remove zeros à direita da vírgula
			if ( valor.indexOf( ',' ) >= 0 )
			{
				while ( valor.charAt( valor.length - 1 ) == '0' )
				{
					valor = valor.substr( 0, valor.length - 1 );
				}
			}
			// remove vírgula caso seja o último caracter
			if ( valor.charAt( valor.length -1 ) == ',' )
			{
				valor = valor.substr( 0, valor.length - 1 );
			}
			// limpa campo caso valor seja '0'
			if ( valor == '0' )
			{
				valor = '';
			}
			return valor;
		}
		
		/**
		 * Realiza formatações no campo porcentagem que só é possível
		 * quando o foco é retirado do campo. Exemplo: retirar zeros
		 * à direitra da vírgula.
		 * 
		 * @param object
		 * @param integer
		 * @return void
		 */
		function verifica_formato_porcentagem( input, indice )
		{
			input.value = formata_decimal( formata_percentual( input.value ) );
			// limpa campo do valor absoluto caso o valor da porcentagem também esteja vazio
			if ( input.value == '' )
			{
				document.forms['formulario'].elements['fracao[' + indice + '][3]'].value = '';
			}
		}
		
		/**
		 * Submete o formulário caso a porcentagem distribuida
		 * não ultrapasse 100%.
		 * 
		 * @return void
		 */
		function submeter_formulario()
		{
			// verifica se o valor do montante foi ultrapassado
			if ( distribuido_percentual_inicial < 0 )
			{			
				if ( distribuido_percentual < distribuido_percentual_inicial )
				{
					alert( 'Atenção. Os valores distribuídos ultrapassam o montante.' );
					return;
				}
			}
			else if ( distribuido_percentual > 100 )
			{
				alert( 'Atenção. Os valores distribuídos ultrapassam o montante.' );
				return;
			}
			// FIM verifica se o valor do montante foi ultrapassado
			houve_modificacao = false; // evita que seja exibida um mensagem confirmando a submissão
			document.formulario.submit();
		}
		
		/**
		 * Método chamada quanto o usuário quer sair da tela. Caso o
		 * formulário tenha sofrido alguma modificação um aviso é
		 * retornado.
		 * 
		 * @return string
		 */
		function sair_sem_salvar()
		{
			if ( houve_modificacao == true )
			{
				return 'Atenção. Existem dados do formulário que não foram guardados.';
			}
		}
		
		// define método para verificação de saída
		window.onbeforeunload = sair_sem_salvar;
		
	-->
	
</script>