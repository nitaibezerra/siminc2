<?php
	
class Declaracao_Pagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     * 
  CONSTRAINT pk_declaracao_pagamento PRIMARY KEY (decid)
)
     */
    protected $stNomeTabela = "maismedicos.declaracao_pagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "decid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'decid' => null, 
									  	'dtregistro' => null, 
									  	'nucpf' => null, 
									  	'dtnascimento' => null, 
									  	'tpbolsa' => null, 
									  	'nubeneficio' => null, 
									  	'nuip' => null, 
									  );
    
    public function modeloDeclaracaoEbserh($post = null)
    {
    	if($post) extract($post);
    	
    	$rsBeneficiario = $this->recuperaBeneficiario($post);
    	$rsPagamentos = $this->recuperaPagamentosBeneficiario($post);
    	
    	if($rsBeneficiario){
    		$htmlTexto = '
    				<div style="width: 100%; text-align: justify; text-justify: newspaper;">
					<div style="padding:10px;">
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						Informa-se, para os devidos fins, que <b>'.$rsBeneficiario['nome'].'</b>, portador(a)
						do CPF <b>'.formatar_cpf($rsBeneficiario['cpf']).'</b>, data de nascimento <b>'.$rsBeneficiario['nascimento'].'</b>, exerce a função de <b>'.$rsBeneficiario['tipo'].'</b>
						no âmbito do Projeto Mais Médicos para o Brasil, e percebe benefício sob registro nº <b>'.$rsBeneficiario['beneficio'].'</b>,
						na modalidade de bolsa, nos termos da Lei Nº 12.871/2013, conforme demonstrativo abaixo.
					</div>
				</div>
    				';
    	}
    	
    	if($rsPagamentos){
    		 
    		$htmlPagamentos .= '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;">
    									<tr><td colspan="4"><center><b>Histórico de pagamentos</b></center></td></tr>
										<tr>
											<td style="text-align:left;"><b>Beneficiário</b></td>
											<td style="text-align:center;"><b>Função</b></td>
											<td style="text-align:right;"><b>Valor em reais</b></td>
											<td style="text-align:center;"><b>Mês de atividade</b></td>
											<td style="text-align:center;"><b>Data de pagamento</b></td>
										</tr>';
    		 
    		foreach($rsPagamentos as $indice => $pagamento){
    	
    			$totalPagamento += $pagamento['valor'];
    			$htmlPagamentos .= '<tr>
										<td style="text-align:left;">'.$pagamento['tutnome'].'</td>
										<td style="text-align:center;">'.$pagamento['funcao'].'</td>
										<td style="color:blue;text-align:right;">'.formata_valor($pagamento['valor']).'</td>
										<td style="text-align:center;">'.$pagamento['periodo'].'</td>
										<td style="text-align:center;">'.$pagamento['pagamento'].'</td>
									</tr>';
    		}
    		 
    		$htmlPagamentos .= '<tr>
									<td colspan="2" style="text-align:left;"><b>Total:</b></td>
									<td style="color:blue;text-align:right;"><b>'.formata_valor($totalPagamento).'</b></td>
									<td></td>
								</tr>
							</table>';
    		 
    	}else{
    		 
    		$htmlPagamentos .= '<p><font color="red">Não foram encontrados Registros.</font></p>';
    	}
    	
    	$htmlAutenticacao = '<div style="text-align:left;font-size:9px;">';
//     	$htmlAutenticacao .= '<p>Total:&nbsp;'.count($rsPagamentos).' registros,&nbsp;';
    	$htmlAutenticacao .= '<p>Ano de exercício: '.$exercicio_declaracao.'</p>';
    	$htmlAutenticacao .= 'Gerado em:&nbsp;'.date('d/m/Y H:i:s').'</p>';
    	$htmlAutenticacao .= '<p>Número de Autenticação:&nbsp;'.$_SESSION['maismedicos']['declaracao_pagamento']['cod_autenticacao'].'</p>';
    	$htmlAutenticacao .= '</div>';
    	
    	$htmlDeclaracao = '
    			<center><h2>COMPROVANTE DE RENDIMENTOS PAGOS</h2></center>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;">
    				<tr>
    					<td align="left" valign="top"><img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0"></td>
    					<td align="left" valign="top">
    						<font size=""><b>MINISTÉRIO DA EDUCAÇÃO</b></font><br/>
    						<font size=""><b>EMPRESA BRASILEIRA DE SERVIÇOS HOSPITALARES - EBSERH</b></font><br/>
    						<b>Imposto sobre a Renda da Pessoa Física</b><br/>
    						Exercício de '.$exercicio_declaracao.'
    					</td>
		    			<td align="left" valign="top" style="top:0px;">
		    				<b>Comprovante de Rendimentos Pagos e de Imposto sobre a
							Renda Retido na Fonte</b><br/><br/><br/>
							Ano-calendário de '.($exercicio_declaracao+1).'
		    			</td>
    				</tr>
					<tr><td colspan="3"><font style="font-size:9px">Verifique as condições e o prazo para a apresentação da Declaração do Imposto sobre a Renda da Pessoa Física para este ano-calendário no sítio da Secretaria da Receita Federal do Brasil na Internet, no endereço <a href="www.receita.fazenda.gov.br" target="_blank">www.receita.fazenda.gov.br</a>.</font></td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td colspan="2"><b>1. Fonte Pagadora Pessoa Jurídica</b></td></tr>
					<tr>
						<td width="50%">CNPJ: 15.126.437/0001-43</td>
						<td>Nome empresa: Empresa Brasileira de Serviços Hospitalares - EBSERH</td>
					</tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td colspan="2"><b>2. Pessoa Física Beneficiária dos Rendimentos</b></td></tr>
					<tr>
						<td width="50%">CPF: '.formatar_cpf($rsBeneficiario['cpf']).'</td>
						<td>Nome: '.$rsBeneficiario['nome'].'</td>
					</tr>
					<tr><td colspan="2">Natureza do rendimento: Bolsa/Doação</td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td><b>3. Rendimentos Tributáveis, Deduções e Imposto sobre a Renda Retido na Fonte</b></td><td width="150">Valores em reais</td></tr>
					<tr><td>1. Total dos rendimentos (inclusive férias)</td><td>0,00</td></tr>
					<tr><td>2. Contribuição previdenciária oficial</td><td>0,00</td></tr>
					<tr><td>3. Contribuição a entidades de previdência complementar e a fundos de aposentadoria prog. individual (Fapi)</td><td>0,00</td></tr>
					<tr><td>4. Pensão alimentícia</td><td>0,00</td></tr>
					<tr><td>5. Imposto sobre a renda retido na fonte</td><td>0,00</td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td><b>4. Rendimentos Isentos e Não Tributáveis</b></td><td width="150">Valores em reais</td></tr>
					<tr><td>1. Parcela isenta dos proventos de aposentadoria, reserva remunerada, reforma e pensão (65 anos ou mais)</td><td>0,00</td></tr>
					<tr><td>2. Diárias e ajudas de custo</td><td>0,00</td></tr>
					<tr><td>3. Pensão e proventos de aposentadoria ou reforma por moléstia grave; proventos de aposentadoria ou reforma por acidente em serviço</td><td>0,00</td></tr>
					<tr><td>4. Lucros e dividendos, apurados a partir de 1996, pagos por pessoa jurídica (lucro real, presumido ou arbitrado)</td><td>0,00</td></tr>
					<tr><td>5. Valores pagos ao titular ou sócio da microempresa ou empresa de pequeno porte, exceto pro labore, aluguéis ou serviços prestados</td><td>0,00</td></tr>
					<tr><td>6. Indenizações por rescisão de contrato de trabalho, inclusive a título de PDV e por acidente de trabalho</td><td>0,00</td></tr>
					<tr><td>7. Outros (BOLSA)</td><td>'.formata_valor($totalPagamento).'</td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td><b>5. Rendimentos Sujeitos à Tributação Exclusiva (rendimento líquido)</b></td><td width="150">Valores em reais</td></tr>
					<tr><td>1. Décimo terceiro salário</td><td>0,00</td></tr>
					<tr><td>2. Imposto sobre a renda retido na fonte sobre 13º salário</td><td>0,00</td></tr>
					<tr><td>3. Outros</td><td>0,00</td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td><b>6. Rendimentos Recebidos Acumuladamente - Art. 12-A da Lei nº 7.713, de 1988 (sujeitos à tributação exclusiva)</b></td><td width="150">Valores em reais</td></tr>
					<tr><td colspan="2">6.1 Número do processo: - </td></tr>
					<tr><td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Quantidade de meses: - </td></tr>
					<tr><td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Natureza do Rendimento: - </td></tr>
					<tr><td>1. Total dos rendimentos tributáveis (inclusive férias e décimo terceiro salário)</td><td>0,00</td></tr>
					<tr><td>2. Exclusão: Despesas com a ação judicial</td><td>0,00</td></tr>
					<tr><td>3. Dedução: Contribuição previdenciária oficial</td><td>0,00</td></tr>
					<tr><td>4. Dedução: Pensão alimentícia</td><td>0,00</td></tr>
					<tr><td>5. Imposto sobre a renda retido na fonte</td><td>0,00</td></tr>
					<tr><td>6. Rendimentos isentos de pensão, proventos de aposentadoria ou reforma por moléstia grave ou aposentadoria ou reforma por acidente em serviço</td><td>0,00</td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td><b>7. Informações Complementares</b></td></tr>
					<tr><td>Emitido eletrônicamente pelo SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação, gerado em '.date('d/m/Y H:i:s').', número de autenticação:&nbsp;'.$_SESSION['maismedicos']['declaracao_pagamento']['cod_autenticacao'].'</td></tr>
				</table>
    			<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:10px;border: 1px solid black;margin-top:5px;">
					<tr><td colspan="2"><b>8. Responsável pelas Informações</b></td></tr>
					<tr>
						<td>Nome: </td>
						<td>Data: '.date('d/m/Y').'</td>
						<td>Assinatura: </td>
					</tr>
    			</table>';
    	
    	$htmlAprovadoLei = '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="100%" style="font-size:9px;">
								<tr><td>Aprovado pela Instrução Normativa RFB nº 1.522, de 05 de dezembro de 2014.</td></tr>
							</table>';
    	
    	$html = ''.$htmlDeclaracao.''.$htmlAprovadoLei.'<br/><br/><br/><center><h2>DECLARAÇÃO DE PAGAMENTO</h2></center>'.
    			$htmlTexto.'<br/>'.$htmlPagamentos.'<br/>'.$htmlAutenticacao;
    	
    	return $html;
    }
    
    public function recuperaBeneficiario($post = null)
    {
    	if($post) extract($post);
    	
    	if($nucpf){
    		$nucpftemp = str_replace(array('.','-'), '', $nucpf);
    		$arWhere[] = " tut.tutcpf = '{$nucpftemp}'";
    	}
    	
    	if($dtnascimento){
    		$dtnascimentotemp = formata_data_sql($dtnascimento);
    		$arWhere[] = " to_char(tut.tutdatanascimento, 'YYYY-MM-DD') = '{$dtnascimentotemp}' ";
    	}
    	
    	if($nubeneficio){
    		$arWhere[] = " det.nu_nib = '{$nubeneficio}' ";
    	}
    	
    	$sql = "select
			    	tut.tutcpf as cpf,
			    	tut.tutnome as nome,
			    	to_char(tut.tutdatanascimento,'DD/MM/YYYY') as nascimento,
			    	det.nu_nib as beneficio,
			    	case when tut.tuttipo = 'S' then 'Supervisão'
			    	when tut.tuttipo = 'T' then 'Tutoria'
			    	else tut.tuttipo end as tipo
		    	from maismedicos.tutor tut
		    	join maismedicos.remessadetalhe det on tut.tutcpf = det.nu_cpf
		    	where tut.tutcpf = '{$nucpftemp}'
		    	and to_char(tut.tutdatanascimento, 'YYYY-MM-DD') = '{$dtnascimentotemp}'
		    	and det.nu_nib = '{$nubeneficio}'";
    	 
    	return $this->pegaLinha($sql);
    }
    
    public function recuperaPagamentosBeneficiario($post = null)
    {
    	if($post) extract($post);
    	
    	if($nucpf){
    		$nucpftemp = str_replace(array('.','-'), '', $nucpf);
    		$arWhere[] = " tut.tutcpf = '{$nucpftemp}'";
    	}
    	
    	if($dtnascimento){
    		$dtnascimentotemp = formata_data_sql($dtnascimento);
    		$arWhere[] = " to_char(tut.tutdatanascimento, 'YYYY-MM-DD') = '{$dtnascimentotemp}' ";
    	}
    	
    	if($exercicio_declaracao){
    		$arWhere[] = "to_char(dt_envio_email, 'YYYY') = '{$exercicio_declaracao}'";
    	}
    	
    	if($nubeneficio){
    		$arWhere[] = " det.nu_nib = '{$nubeneficio}' ";
    	}
    	
    	// Registra emicao declaracao
    	if(empty($_SESSION['maismedicos']['declaracao_pagamento']['id_autenticacao'])){
    	
    		$sqlInserLog = "insert into maismedicos.declaracao_pagamento
				    			(nucpf,dtnascimento,tpbolsa,nubeneficio,nuip)
				    		values
				    			('{$nucpftemp}','{$dtnascimentotemp}','','{$nubeneficio}','{$nuip}') returning decid;";
    			
    		$idLog = $this->pegaUm($sqlInserLog);
    		$this->commit();
    				
    	}    	
    	if($idLog)
    		$_SESSION['maismedicos']['declaracao_pagamento']['id_autenticacao'] = $idLog;
		else
			$idLog = $_SESSION['maismedicos']['declaracao_pagamento']['id_autenticacao'];
		
		$sqlAutenticacao = "select to_char(dtregistro,'YYYYMMDDHHIISS') || '-' || nubeneficio || '-' || lpad(decid::varchar, 8, '0') from maismedicos.declaracao_pagamento where decid = {$idLog}";
		$autenticacao = $this->pegaUm($sqlAutenticacao);		
		$_SESSION['maismedicos']['declaracao_pagamento']['cod_autenticacao'] = $autenticacao;
    	
    	$sql = "select
   				tut.tutnome,
   				case when tut.tuttipo = 'T'
   					then 'Tutor'
   					else 'Supervisor'
   				end as funcao,
   				case when tut.tuttipo = 'T'
   					then '5000'
   					else '4000'
   				end as valor,
				to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'MM/YYYY') as periodo,
    			to_char(dt_envio_email, 'DD/MM/YYYY') as pagamento
   			from
   				maismedicos.tutor tut
   			inner join
   				maismedicos.universidade uni ON uni.uniid = tut.uniid
   			inner join
   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid and det.cs_ocorrencia = '0000'
   			inner join
   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
   			left join
   				maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
			left join
				maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'
   			left join
   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia
			where
				1=1
   			".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
   			order by
   				tut.tutnome, to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'YYYYMM') asc";
    	
    	return $this->carregar($sql);
    }
}