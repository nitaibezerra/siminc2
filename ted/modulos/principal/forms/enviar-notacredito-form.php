<form class="form-horizontal"
      name="formEnviarPagamento"
      id="formEnviarPagamento"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

      <?= $this->element->tcpid; ?>
      
    <input type="hidden" name="funcao" id="funcao" value="fndeenviarnc" />
      
    <div class="form-group">
        <label class="control-label col-md-3" for="sigefusername">Usuário do SIGEF:</label>
        <div class="col-md-9">
            <?= $this->element->sigefusername; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="sigefpassword">Senha do SIGEF:</label>
        <div class="col-md-9">
            <?= $this->element->sigefpassword; ?>
        </div>
    </div>
    
    <?php 
    $tcpid = $_GET['ted'];
    $dadosNC = consultarNotasDeCredito($tcpid, true, false);
    //ver($dadosNC, d);

    if (empty($dadosNC)) {
    	echo "<p style=\"text-align:center\">Sem NC para disponibilizar para pagamento</p>";
    
    	//Ser não tiver nenhuma NC para pagamento, verifica se ja existe alguma nc paga
    	$dadosNC = pagamentosSolicitados($tcpid, true, true);
    	$desabilitaBotao = true;
    	if (!$dadosNC) return;
    }
    
    ?>
    <table class="table table-bordered table-striped table-responsive table-condensed">
        <tr class="SubTituloCentro">
            <th colspan="11">Requisições de Pagamentos</th>
        </tr>
		    <?php 
		    $output = '';
		    $output.= '<tr>';
		    $output.= '<td>&nbsp;</td>
                        <td>Nº parcela</td>
				        <td>Mês da<br />liberação</td>
				        <td>Ano</td>
				        <td>PTRES</td>
				        <td>PI</td>
				        <td>Natureza da Despesa</td>
				        <td>Valor (R$)</td>
				        <td>Cadastrada<br />no SIGEF?</td>
				        <td>Último retorno SIGEF</td>
				        <td>Nota de crédito</td>
				        <td>Cadastro no SIGEF em</td>';
		    
		    $output .= '</tr>';
		    
		    $i = 0;
		    $meses = array();
		    $meses[++$i] = "Janeiro";
		    $meses[++$i] = "Fevereiro";
		    $meses[++$i] = "Março";
		    $meses[++$i] = "Abril";
		    $meses[++$i] = "Maio";
		    $meses[++$i] = "Junho";
		    $meses[++$i] = "Julho";
		    $meses[++$i] = "Agosto";
		    $meses[++$i] = "Setembro";
		    $meses[++$i] = "Outubro";
		    $meses[++$i] = "Novembro";
		    $meses[++$i] = "Dezembro";
		    
		    $proids = array();
            //ver($dadosNC);
		    foreach ($dadosNC as $nc) {
		    	/**
		    	 * Solução paleativa para linhas duplicadas
		    	 */
		    	if (in_array($nc['proid'], $proids)) {
		    		continue;
		    	}
		    	array_push($proids, $nc['proid']);
		    	$nc['valor_pagamento'] = formata_valor($nc['valor_pagamento']);
		    	$retornoSIGEFStyle = ('-' == $nc['ppaultimoretornosigef']?'':' style="text-align:left"');
		    	$ultimoRetorno = utf8_decode($nc['ppaultimoretornosigef']);
		    	$output.= '<tr>';

		    	if ($nc['ppacadastradosigef'] == 'Sim') {
		    		$output.= "<td><input type='checkbox' disabled='disabled' name='proid[]' value='{$nc['proid']}' id='{$nc['proid']}' /></td>";
		    	} else {
		    		$output.= "<td><input type='checkbox' name='proid[]' value='{$nc['proid']}' id='{$nc['proid']}' /></td>";
		    	}
		    	$output.= "<td class='text-center'>{$nc['numero_parcela']}</td>
		    				<td>{$meses[$nc['mes_pagamento']]}</td>
		    				<td>{$nc['ano_parcela']}</td>
		    				<td>{$nc['ptres']}</td>
		    				<td>{$nc['plano_interno']}</td>
		    				<td class='text-center'>{$nc['natureza_despesa']}</td>
		    				<td>{$nc['valor_pagamento']}</td>
		    				<td class='text-center'>{$nc['ppacadastradosigef']}</td>
		    				<td{$retornoSIGEFStyle}>{$ultimoRetorno}</td>
		    				<td class='text-center'>{$nc['nota_credito']}</td>
		    				<td class='text-center'>{$nc['ppadata']}</td>
		    				";

			    $output .= "</tr>";
			}
			echo $output;
		    ?>
	</table>
	
	<button type="submit" class="btn btn-primary" id="enviarNC">Disponibilizar para pagamento</button>
</form>