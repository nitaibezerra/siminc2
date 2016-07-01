<?php

class Pagamento_Autorizacao extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "maismedicos.pagamento_autorizacao";
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "apgid" );
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos     = array(
								  'apgid' => null,
								  'lcbid' => null,
								  'rmdid' => null,
								  'benid' => null,
								  'apgcpf' => null,
								  'apgnome' => null,
								  'apgtipo' => null,
								  'apgmes' => null,
								  'apgano' => null,  
								  'apgcpfresponsavel' => null,
								  'apgnomeresponsavel' => null,
								  'apgstatus' => null,
								  'apgjustificativa' => null,
								  'apgenviado' => null,
								  'apgdtregistro' => null,  
								  'cpfvalidacoordenador' => null,
								  'dtvalidacoordenador' => null,
								  'cpfvalidagestor' => null,
								  'dtvalidagestor' => null,
									);
	
	public function validaGestorPagamento($post=null)
	{
		if($post['benid']>0 && $post['mes']>0 && $post['ano']>0){
			
			$sql = "select apgid, cpfvalidagestor from {$this->stNomeTabela} 
			where apgstatus = 'A' and benid = {$post['benid']}
			and apgmes = '{$post['mes']}' and apgano = '{$post['ano']}'";
			
			$autorizacao = $this->pegaLinha($sql);
			
			if($autorizacao['apgid']>0){
				
				if(empty($autorizacao['cpfvalidagestor']) || $post['status']=='I'){
					$sql = "update {$this->stNomeTabela} set cpfvalidagestor = '{$_SESSION['usucpf']}', dtvalidagestor = now(), 
								apgstatus = '{$post['status']}', apgjustificativagestor = '{$post['justificativa']}'
							where apgid = {$autorizacao['apgid']}";
				}
				
			}else{
				
				$sql = "insert into {$this->stNomeTabela}
							(benid, apgcpf, apgnome, apgtipo, apgmes, apgano, apgcpfresponsavel, apgnomeresponsavel,
							apgjustificativa,cpfvalidagestor,dtvalidagestor, apgjustificativagestor)
							select benid, bencpf, bennome, funid, '{$post['mes']}', '{$post['ano']}', '{$_SESSION['usucpf']}', '{$_SESSION['usunome']}',
							'{$post['justificativa']}', '{$_SESSION['usucpf']}', now(), '{$post['justificativa']}'
							from maismedicos.pagamento_beneficiario where benid = {$post['benid']}";
			}
			
			if($sql){
				$this->executar($sql);
				$this->commit();
				return true;
			}
			return false;
		}
	}
	
	public function validaCoordenadorPagamento($post=null)
	{
		if($post['benid']>0 && $post['mes']>0 && $post['ano']>0){
			
			$sql = "select apgid, cpfvalidacoordenador from {$this->stNomeTabela} 
			where apgstatus = 'A' and benid = {$post['benid']}
			and apgmes = '{$post['mes']}' and apgano = '{$post['ano']}'";
			
			$autorizacao = $this->pegaLinha($sql);
			
			if($autorizacao['apgid']>0){
				
				if(empty($autorizacao['cpfvalidacoordenador']) || $post['status']=='I'){
					$sql = "update {$this->stNomeTabela} set cpfvalidacoordenador = '{$_SESSION['usucpf']}', 
								dtvalidacoordenador = now(), apgstatus = '{$post['status']}', apgjustificativacoord = '{$post['justificativa']}'
							where apgid = {$autorizacao['apgid']}";
				}
				
			}else{
				
				$sql = "insert into {$this->stNomeTabela}
							(benid, apgcpf, apgnome, apgtipo, apgmes, apgano, apgcpfresponsavel, apgnomeresponsavel,
							apgjustificativa,cpfvalidacoordenador,dtvalidacoordenador, apgjustificativacoord)
							select benid, bencpf, bennome, funid, '{$post['mes']}', '{$post['ano']}', '{$_SESSION['usucpf']}', '{$_SESSION['usunome']}',
							'{$post['justificativa']}', '{$_SESSION['usucpf']}', now(), '{$post['justificativa']}'
							from maismedicos.pagamento_beneficiario where benid = {$post['benid']}";
			}
			
			if($sql){
				$this->executar($sql);
				$this->commit();
				return true;
			}
			return false;
		}
	}
	
	public function montaFormJustificativa()
	{
		$html = '
			<input type="hidden" name="img_modal" value=""/>
			<input type="hidden" name="benid_modal" value=""/>
			<input type="hidden" name="metodo_modal" value=""/>
			<input type="hidden" name="mes_modal" value=""/>
			<input type="hidden" name="ano_modal" value=""/>
			<input type="hidden" name="status_modal" value=""/>
			<center>
				<h3>Justificativa</h3>
				<textarea name="justificativa" cols="20" rows="8"></textarea>
				<p>&nbsp;</p>
				<p>
				<input type="button" value="Validar" class="btnConfirmaPagamento" />&nbsp;
				<input type="button" value="Fechar" class="btnFecharModal" />
				</p>
			</center>
			';
		echo $html;
	}
	
	public function montaViewPlano()
	{
		$sql = "select * from maismedicos.planotrabalho where ptrstatus = 'A' and ptrcpf = '{$_SESSION['usucpf']}' 
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." order by ptrid desc";
		
    	return $this->pegaLinha($sql);
    	
		$html .= '<h2>Plano de Trabalho</h1>';
		echo $html;
	}
	
	public function recuperaPlanoTrabalho($post = null)
	{
		if($post['mes'])
			$arWhere[] = "ptrmes = '{$post['mes']}'";
		if($post['ano'])
			$arWhere[] = "ptrano = '{$post['ano']}'";
		if($post['cpf'])
			$arWhere[] = "ptrcpf = '{$post['cpf']}'";
		if($post['ptrid'])
			$arWhere[] = "p.ptrid = '{$post['ptrid']}'";
	
		$sql = "select p.*, e.ptedtenvio, e.pteobs, e.pteid, e.ptemes, e.pteano, e.ptesemestre,
				 a.apgid, apgdtregistro, apgcpfresponsavel, apgnomeresponsavel, apgjustificativa,
				apgevidencia,ptevalida,ptedtvalidacao,ptecpfvalidacao,ptenomevalidacao,pteobsvalidacao,
				pteevidencia
				from maismedicos.planotrabalho p
    			left join maismedicos.planotrabalho_execucao e on e.ptrid = p.ptrid
					and e.ptestatus = 'A'
				left join maismedicos.pagamento_autorizacao a on a.apgid = e.apgid
					and a.apgmes::integer = e.ptemes::integer
					and a.apgano::integer = e.pteano::integer
					and a.apgstatus = 'A'  
    			where ptrstatus = 'A'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
		//echo '</pre>';var_dump($sql);die;
		return $this->pegaLinha($sql);
	}
	
	public function montaDetalhePlanoTrabalho($post = null, $plano = null)
	{
		
		if($plano['ptrid']) $post['ptrid'] = $plano['ptrid'];
		if($plano['ptrcpf']) $post['ptrcpf'] = $plano['ptrcpf'];
		
		if($plano){
			
			$html = '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
					    <tr>
					        <td class="subtitulodireita" width="150" valign="top">Nome</td>
					        <td valign="top">'.formatar_cpf($plano['ptrcpf']).' - '.$plano['ptrnome'].'</td>
					    </tr>
					    <tr>
					        <td class="subtitulodireita" valign="top">Mês de Referência</td>
					        <td valign="top">('.$plano['ptrmes'].'/'.$plano['ptrano'].')'.'</td>
					    </tr>
					    <tr>
					        <td class="subtitulodireita" valign="top">Período</td>
					        <td valign="top">'.$plano['ptrsemestre'].'º semestre de '.$plano['ptrano'].'</td>
					    </tr>
						</table>';
	
			$html .= $this->montaDetalheVisita($post);
			$html .= $this->montaDetalheAtividade($post);
			$html .= $this->montaDetalheObservacao($post);
    	
		}
		return $html;
	}
	
	public function montaDetalheObservacao($post)
	{
		if($post['cpf']){
			$arWhere[] = "p.ptrcpf = '{$post['cpf']}'";
		}
		if($post['mes']){
			$arWhere[] = "ptrmes = '{$post['mes']}'";
		}
		if($post['ano']){
			$arWhere[] = "ptrano = '{$post['ano']}'";
		}
		$sql = "select ptrobs, pteobs, ptrdtenvio, ptedtenvio from maismedicos.planotrabalho p
				left join maismedicos.planotrabalho_execucao e on e.ptrid = p.ptrid
					and e.ptestatus = 'A'
				where p.ptrstatus = 'A'".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
		
		$rs = $this->pegaLinha($sql);
		
		if($rs){
			$html .= '<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
						<tr><td class="subtitulocentro">Observação</td></tr>
						<tr><td>
							<p><b>Plano/Ajuste (Status: '.($rs['ptrdtenvio'] ? '<font color="green">Enviado em '.formata_data($rs['ptrdtenvio']) : '<font color="red">Não enviado').'</font>)
							: </b></p>'.($rs['ptrobs'] ? $rs['ptrobs'] : 'Sem observação').'
						</td></tr>
						<tr height="25"><td>&nbsp;</td></tr>
						<tr><td>
							<p><b>Execução (Status: '.($rs['ptedtenvio'] ? '<font color="green">Enviado em '.formata_data($rs['ptedtenvio']) : '<font color="red">Não enviado').'</font>)
							: </b></p>'.($rs['pteobs'] ? $rs['pteobs'] : 'Sem observação').'
						</td></tr>
					  </table>
					';
		}
		
		return $html;
	}
	
	public function montaDetalheVisita($post)
	{
		$post['mes'] = str_pad($post['mes'],2,'0', STR_PAD_LEFT);
		
		$sql = "select v.*, ifenome, ifcnome from maismedicos.planotrabalho_visitaprogramada v
				join maismedicos.planotrabalho p on p.ptrid = v.ptrid
				left join maismedicos.ifes u on u.ifeid = v.ifeid
				left join maismedicos.ifes_campi c on c.ifcid = v.ifcid
				where ptvstatus = 'A'
				and ( (to_char(ptvdtprevini,'YYYYMM')>='{$post['ano']}{$post['mes']}'
				and to_char(ptvdtprevfim,'YYYYMM')<='{$post['ano']}{$post['mes']}') or ptvdtenvioexec is null)
				and p.ptrcpf = '{$post['ptrcpf']}'";
			
		$arVisita = $this->carregar($sql);
		
		if($arVisita){
			$html .= '<table align="center" class="tabela" id="dados_visita" width="100%" bgcolor="#f5f5f5" style="margin-top:15px;">
						<thead>
						<tr class="subtitulocentro"><td colspan="7">Visita Programada</td></tr>
						<tr>
							<th>Instituição</th>
							<th>Campus</th>
							<th>Período Previsto</th>
							<th>Obs</th>
							<th>Anexo</th>
							<th>Período Executado</th>
							<th>Obs Execução</th>
							<th>Anexo Execução</th>
							<th></th>
						</tr></thead><tbody>';
		
			foreach($arVisita as $key => $visita){
				
				$arDataIni = $visita['ptvdtprevini'] ? explode('-',$visita['ptvdtprevini']) : array();
				$arDataFim = $visita['ptvdtprevfim'] ? explode('-',$visita['ptvdtprevfim']) : array();
				$data1 = $post['ano'].$post['mes'];
				$data2 = $arDataIni[0].$arDataIni[1];
				$data3 = $arDataFim[0].$arDataFim[1];
				$varHtml = $data1>=$data2 &&  $data1<=$data3 ? 'html' : 'htmlOutros';
				//ver($data1, $data2, $data3);
				${$varHtml} .= '<tr id="tr_visita_ '.$key.'" style="">
						<td width="10%" valign="top" align="left">'.$visita['ifenome'].'</td>
						<td width="10%" valign="top" align="left">'.$visita['ifcnome'].'</td>
						<td width="11%" valign="top" align="left">
						'.formata_data($visita['ptvdtprevini']).'&nbsp;até<br/>
						'.formata_data($visita['ptvdtprevfim']).'
						</td><td width="21%" valign="top" align="left">
						'.$visita['ptvobs'].'</td><td width="5%" valign="top">';
		    								       
				if($visita['arqid']){
					${$varHtml} .= '<center><img src="../imagens/clipe.gif" id="'.$visita['arqid'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
				}else{
					${$varHtml} .= '<center>-</center>';
				}
			
				${$varHtml} .= '</td><td width="12%" valign="top" align="left">'.formata_data($visita['ptvdtexecini']).'
							&nbsp;até<br/>'.formata_data($visita['ptvdtexecfim']).'</td><td width="21%" valign="top" align="left">
							'.$visita['ptvobsexecutado'].'</td><td width="5%" valign="top" align="left">';
		
				if($visita['arqidexecutado']){
					${$varHtml} .= '<center><img src="../imagens/clipe.gif" id="'.$visita['arqidexecutado'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
				}
		
				${$varHtml} .= '</td><td>';
								
				if($visita['ptvdtenvioexec']){
					${$varHtml} .= '<img src="../imagens/check_checklist.png" title="Executado" />';
				}else{
					${$varHtml} .= '<img src="../imagens/excluir_2.gif" title="Executado" />';
				}
				${$varHtml} .= '</tr>';
			}
			if($htmlOutros){
				$html .= '<tr><td class="subtituloCentro" colspan="7">Outros Meses</td></tr>'.$htmlOutros;
			}
    		$html .= '</table>';
    	}
    	return $html;
	}
	
	public function montaDetalheAtividade($post)
	{
		$post['mes'] = str_pad($post['mes'],2,'0', STR_PAD_LEFT);
		
		$sql = "select v.* , ifenome, ifcnome from maismedicos.planotrabalho_atividadeacompanhamento v
				join maismedicos.planotrabalho p on p.ptrid = v.ptrid
				left join maismedicos.ifes u on u.ifeid = v.ifeid
				left join maismedicos.ifes_campi c on c.ifcid = v.ifcid
				where ptastatus = 'A'
				and (to_char(ptadtprevisto,'YYYYMM')='{$post['ano']}{$post['mes']}' or ptadtenvioexec is null)
				and p.ptrcpf = '{$post['ptrcpf']}'";
		
		$arAtividade = $this->carregar($sql);
		
		if($arAtividade){
			
			$html .= '<table align="center" class="tabela" id="dados_atividade" width="100%" bgcolor="#f5f5f5" style="margin-top:15px;">
					<thead>
					<tr class="subtitulocentro"><td colspan="9">Atividades de Acompanhamento</td></tr>
							<tr>
							<th>Instituição</th>
							<th>Campus</th>
							<th>Atividade</th>
							<th>Período Previsto</th>
							<th>Tipo</th>
							<th>Descrição</th>
							<th>Anexo</th>
	    			<th>Data Execução</th>
	    			<th>Obs Execução</th>
	    				<th>Anexo Execução</th>
	    				<th></th></tr></thead><tbody>';
				
			foreach($arAtividade as $key => $atividade){
				
				$arData = $atividade['ptadtprevisto'] ? explode('-',$atividade['ptadtprevisto']) : array();
				$data1 = $post['ano'].$post['mes'];
				$data2 = $arData[0].$arData[1];
				$varHtml = $data1==$data2 ? 'html' : 'htmlOutros';
				
				${$varHtml} .= '<tr id="tr_atividade_'.$key.'">
							<td width="8%" valign="top" align="left">'.$atividade['ifenome'].'</td>
							<td width="8%" valign="top" align="left">'.$atividade['ifcnome'].'</td>
							<td width="8%" valign="top" align="left">'.$atividade['ptanome'].'</td>
							<td width="10%" valign="top" align="left">'.($atividade['ptadtprevisto'] ? formata_data($atividade['ptadtprevisto']) : '-').'</td>
							<td width="7%" valign="top" align="left">'.$atividade['ptatipo'].'</td>
							<td width="14%" valign="top" align="left">'.$atividade['ptadsc'].'</td>
							<td width="5%" valign="top">';
			
				if($atividade['arqid']){
					${$varHtml} .= '<center><img src="../imagens/clipe.gif" id="'.$atividade['arqid'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
				}else{
					${$varHtml} .= '	<center>-</center>';
				}
				${$varHtml} .= '</td><td width="10%" valign="top">'.formata_data($atividade['ptadtexecutado']).'</td>
						<td width="20%" valign="top">'.$atividade['ptaobsexecutado'].'</td><td width="5%" valign="top">';
			
				if($atividade['arqidexecutado']){
					${$varHtml} .= '<center><img src="../imagens/clipe.gif" id="'.$atividade['arqid'].'" class="btnDownloadAnexo" style="cursor:pointer;"/></center>';
				}
	    			
				${$varHtml} .= '</td><td>';
	    			
				if($atividade['ptadtenvioexec']){
					${$varHtml} .= '<img src="../imagens/check_checklist.png" title="Executado" />';
				}else{
					${$varHtml} .= '<img src="../imagens/excluir_2.gif" title="Executado" />';
				}
											            					
				${$varHtml} .= '</td></tr>';
			}
			if($htmlOutros){
				$html .= '<tr><td class="subtituloCentro" colspan="9">Outros Meses</td></tr>'.$htmlOutros;
			}
			$html .= '</tbody></table>';
		}
		return $html;
	}
	
	public function validaBolsa($post = null)
	{
		if($post['confirma']){
			return $this->insereAutorizacaoPagamento($post);
		}
		
		$htmlScript = "<script>
					$(function(){
						$('.btnFechar').click(function(){
							$( '#modalDialog' ).dialog('close')
						});
						$('.btnDownloadAnexo').click(function(){
							url = document.location.href+'&anexo=download&arqid='+this.id;
							var popUp = window.open(url, 'popDownloadAnexo', 'height=500,width=400,scrollbars=yes,top=50,left=200');
							popUp.focus();
						});
						$('.btnEnviaValidacao').click(function(){
				
							txt = this.value;
							pagar = txt=='Pagar' ? 'S' : 'N';
							cpf = $('[name=apgcpf]').val();
							mes = $('[name=apgmes]').val();
							ano = $('[name=apgano]').val();
							pteid = $('[name=pteid]').val();
							evidencia = $('[name=apgevidencia]').val();
							justificativa = $('[name=apgjustificativa]').val();
				
							if(confirm(''+txt+' a bolsa para o cpf '+cpf+'?')){
				
								if(!$('[name=apgjustificativa]').val()){
									alert('O campo justificativa é obrigatório!');
									$('[name=apgjustificativa]').focus();
									return false;
								}
								
								
								dados = '&requisicao=validaBolsa&confirma=true&cpf=';
								dados += cpf+'&mes='+mes+'&ano='+ano+'&justificativa='+justificativa;
								dados += '&pagar='+pagar+'&pteid='+pteid+'&evidencia='+evidencia;
								$.ajax({
									url	: document.location.href,
									type	: 'post',
									data	: dados,
									success	: function(e){
										alert(e);
										$( '#modalDialog' ).dialog('close');
										document.location.reload();
										}
									});
							}
						});
					});
					</script>"; 
		
		$plano = $this->recuperaPlanoTrabalho($post);
		
		$htmlPlano .= $this->montaDetalhePlanoTrabalho($post, $plano);
		$html .= $htmlScript.$htmlPlano;
		
		if(empty($plano['pteid'])){
			$html .= '<center>
					<p>
					<img src="../imagens/excluir_2.gif" title="Validado" align="absmiddle"/>
					<font color="red" size="+2">Execução não enviada!</font>
					<p><input type="button" class="btnFechar" value="Fechar" /></p></center></p>';
		}else		
		if(empty($plano['ptevalida'])){

			$html .= '<br/>
					<center>
					<form name="formulario" type="post">
					<input type="hidden" name="apgcpf" value="'.$post['cpf'].'"/>
					<input type="hidden" name="apgmes" value="'.$post['mes'].'"/>
					<input type="hidden" name="apgano" value="'.$post['ano'].'"/>
					<input type="hidden" name="pteid" value="'.$plano['pteid'].'"/>
					<p>
					<b>Justificativa:</b><br/>
					<textarea name="apgjustificativa" style="width:95%;height:120px;"></textarea>
					<br/>
					<br/>
					<input type="button" class="btnEnviaValidacao" value="Pagar" />
					<input type="button" class="btnEnviaValidacao" value="Não Pagar" />
					<input type="button" class="btnFechar" value="Fechar" />
					</form>
					</center>
					';
		}else{
			$html = $htmlScript.'<br/><center>
					<img src="../imagens/check_checklist.png" title="Validado"align="absmiddle" />
					<font color="green" size="+2">Bolsa Validada</font>';
			$html .= '<table align="center" class="tabela" id="dados_atividade" width="100%" bgcolor="#f5f5f5" style="margin-top:15px;">
					<tr><td><b>Resposável:</b> '.formatar_cpf($plano['ptecpfvalidacao']).' - '.$plano['ptenomevalidacao'].'</td></tr>
					<tr><td><b>Data de validação:</b> '.formata_data($plano['ptedtvalidacao']).'</td></tr>
					<tr><td><b>Situação:</b> '.($plano['ptevalida']=='S'?'<font color="green">Pagar':'<font color="red">Não pagar').'</font></td></tr>
					<tr><td class="subtitulocentro">Justificativa</td></tr>
					<tr><td>'.$plano['pteobsvalidacao'].'</td></tr></table></center><p>&nbsp;</p>';
			$html .= $plano['pteevidencia'];
			$html .='<center><p>&nbsp;</p><p><input type="button" class="btnFechar" value="Fechar" /></p></center>';
		}
		echo $html;
		//ver($plano);
		return true;
	}
	
	public function insereAutorizacaoPagamento($post)
	{
		if(!$post['pteid']){
			echo 'Execução não encontrada!';
			return false;
		}
		if(!$this->checkAutorizacaoPagamentoMes($post)){
			if($post['pagar']=='S'){
				$ben = $this->pegarDadosBeneficiario($post['cpf']);
				
				$plano = $this->recuperaPlanoTrabalho($post);				
				$htmlPlano = $this->montaDetalhePlanoTrabalho($post, $plano);
				$htmlPlano = $htmlPlano;
				
				$sql = "insert into maismedicos.pagamento_autorizacao
						(benid, apgcpf, apgnome, apgtipo, apgmes, apgano, apgcpfresponsavel, apgnomeresponsavel,
					apgjustificativa,apgevidencia) values 
						({$ben['benid']}, '{$ben['bencpf']}', '{$ben['bennome']}', '{$ben['bentipo']}', '{$post['mes']}',
						'{$post['ano']}', '{$_SESSION['usucpf']}', '{$_SESSION['usunome']}', '{$post['justificativa']}',
						'{$htmlPlano}')
				returning apgid;";
				
				$apgid = $this->pegaUm($sql);
				$this->commit();
				echo 'Operação realizada com sucesso.';
			}
			$apgid = $apgid ? $apgid : 'null';
			$sql = "update maismedicos.planotrabalho_execucao set
					apgid = {$apgid}, 
					ptevalida = '{$post['pagar']}',
					ptedtvalidacao = now(),
					ptecpfvalidacao = '{$_SESSION['usucpf']}',
					ptenomevalidacao = '{$_SESSION['usunome']}',
					pteobsvalidacao = '{$post['justificativa']}',
					pteevidencia = '{$htmlPlano}'
					where pteid = {$post['pteid']}";
			$this->executar($sql);
			$this->commit();
		}
		else{
			echo "Já existe uma autorização de pagamento para o mês ".$post['mes'].'/'.$post['ano'].'!';
		}
		return true;
		
		/*
		 * 
  apgstatus character(1) DEFAULT 'A'::bpchar,
  apgenviado boolean DEFAULT false,
  apgdtregistro timestamp without time zone DEFAULT now(),  
  cpfvalidacoordenador character(11),
  apgjustificativacoord character varying(500),
  dtvalidacoordenador date,
  cpfvalidagestor character(11),
  apgjustificativagestor character varying(500),
  dtvalidagestor date,
		 */
	}
	
	public function checkAutorizacaoPagamentoMes($post)
	{
		$post['mes'] = str_pad($post['mes'],2,'0', STR_PAD_LEFT);
		
		$sql = "select apgid,apgjustificativa, apgcpfresponsavel, apgnomeresponsavel, apgdtregistro from maismedicos.pagamento_autorizacao apg 
				where apgstatus = 'A'
				and apgcpf = '{$post['cpf']}' and apgmes = '{$post['mes']}' and apgano = '{$post['ano']}'";
		
		return $this->pegaLinha($sql);
	}
	
	public function pegarDadosBeneficiario($cpf)
	{
		$sql = "select * from maismedicos.pagamento_beneficiario where bencpf = '{$cpf}' order by benstatus, benid desc";
		return $this->pegaLinha($sql);
	}
	
	public function checkTutoresSelecionados($arrTutid)
	{
		$sql = "update $this->stNomeTabela set apgenviado = true where rmdid is null and apgcpf in (select bencpf from maismedicos.pagamento_beneficiario where benid in (".implode(",",$arrTutid)."))";
		$this->executar($sql);
		$this->commit();
	}
}
