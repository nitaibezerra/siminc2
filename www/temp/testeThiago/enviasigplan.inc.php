<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Gilberto Arruda Cerqueira Xavier
	Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
	Módulo:exportasigplan.inc
	Finalidade: permitir exporta os dados do SIGPLAN
*/

$modulo = $_REQUEST['modulo'];

include APPRAIZ . 'includes/cabecalho.inc';


$strDir = APPRAIZ . "arquivos/SIGPLAN/exportacao/" ;
$arrFiles = glob( $strDir . "*.*");

foreach( $arrFiles as &$strFile )
{
	$strFile = substr( $strFile , strlen( $strDir ) );
}
?>
<br/>
<?php monta_titulo( 'Envia Arquivo ao Sigplan', '' ); ?>
	<div style="text-align: center;">
		<center>
			<table>
				<tr>
					<td>
						M&eacute;todo para Carga:
					</td>
					<td>
						<select name="MetodoCarga">
							<option selected value="recebePrograma">
								RecebePrograma
							</option>
							<option value="recebeIndicador">
								RecebeIndicador
							</option>
							<option value="recebeRestricaoPrograma">
								RecebeRestricaoPrograma
							</option>
							<option value="recebeAcao">
								RecebeAcao
							</option>
							<option value="recebeRestricaoAcao">
								RecebeRestricaoAcao
							</option>
							<option value="recebeDadoFisico">
								RecebeDadoFisico
							</option>
							<option value="recebeDadoFinanceiro">
								RecebeDadoFinanceiro
							</option>
							<option value="recebeDadoFisicoRAP"> 
								RecebeDadoFisicoRAP
							</option>
							<option value="recebeDadoFinanceiroRAP"> 
								RecebeDadoFinanceiroRAP
							</option>
							<option value="recebeValidacaoTrimestral">
								RecebeValidacaoTrimestral
							</option>
						</select>
					</td>
				</tr>
				<tr>
					<td>
						Usu&aacute;rio:
					</td>
					<td>
						<input type="text" size="50" name="usuario">
					</td>
				</tr>
				<tr>
					<td>
						Senha:
					</td>
					<td>
						<input type="password" size="50" name="senha">
					</td>
				</tr>
				<tr>
					<td>
						Arquivo a ser enviado:
					</td>
					<td>
						<select>
							<?php foreach( $arrFiles as $strFileName ): ?>
								<option>
									<?php print $strFileName ?>
								</option>
							<?php endforeach ?>
						</select>
					</td>
				</tr>
			</table>
		</center>
	</div>
</form>