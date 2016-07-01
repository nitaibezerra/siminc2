<?php

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

/**
 * Classe de Indicação de Avaliador Educacional
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */

class IndAvaliadorEducacional extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.indavaleducacional";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("iaeid");

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
			'iaeid' => null,
			'estuf' => null,
			'iaemunicipioatendido' => null,
			'iaetecnicoprevisto' => null,
			'iaetecnicoindicado' => null,
			'iaesupervisorprevisto' => null,
			'iaesupervisorindicado' => null,
			'iaeexecutivoprevisto' => null,
			'iaeexecutivoindicado' => null,
			'iaesupervgeralprevisto' => null,
			'iaesupervgeralindicado' => null,
			'iaestatus' => null,
            'iaeetapa' => null,
            'iaediretoria' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
			'estuf'
	);

	/**
	 * @name $arCampos
	 * @var array
	 * @access protected
	 * Campos da Tabela
	*/
	protected $arCampos = array(
			'iaeid',
			'estuf',
			'iaemunicipioatendido',
			'iaetecnicoprevisto',
			'iaetecnicoindicado',
			'iaesupervisorprevisto',
			'iaesupervisorindicado',
			'iaeexecutivoprevisto',
			'iaeexecutivoindicado',
			'iaesupervgeralprevisto',
			'iaesupervgeralindicado',
			'iaestatus',
            'iaeetapa',
            'iaediretoria'
	);
	
	protected $arCoresRegiao = array(
			array(
				'1' => '#F0FFF0',
				'2' => '#F5FFFA',
				'3' => '#FFFFF0',
				'4' => '#F0F8FF',
				'5' => '#F8F8FF'
			),
			array(
				'1' => '#F0FFF0',
				'2' => '#F5FFFA',
				'3' => '#FFFFF0',
				'4' => '#F0F8FF',
				'5' => '#F8F8FF'
			)
	);
	
	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	*/
	protected $arAtributosObrigatorios = array(
			'iaeid',
			'estuf',
			'iaemunicipioatendido',
			'iaetecnicoprevisto',
			'iaetecnicoindicado',
			'iaesupervisorprevisto',
			'iaesupervisorindicado',
			'iaeexecutivoprevisto',
			'iaeexecutivoindicado',
			'iaesupervgeralprevisto',
			'iaesupervgeralindicado',
			'iaestatus',
            'iaediretoria'
	);

	public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $funcao = null, $value = null, $size = null) {
		$html = '';
		$selected = '';
	
		$html .= "<select class=\"form-control chosen\" id=\"{$idCampo}\" name=\"{$idCampo}\"";
		if ($funcao != null){
			$html .= "onchange=\"{$funcao}\"";
		}
		$html .= "style=\"";
		if ($size != null){
			$html .= "width: {$size}px; ";
		}
		$html .= "\"";
		$html .= ">";
	
		if (isset ( $htmlOptions ['prompt'] )) {
			$html .= '<option value="">' . strtr ( $htmlOptions ['prompt'], array (
					'<' => '&lt;',
					'>' => '&gt;'
			) ) . "</option>\n";
		}
	
		if ($dados) {
			foreach ( $dados as $data ) {
				if ($idCampo) {
					$selected = ($data ['codigo'] === trim($this->arAtributos[$idCampo]) ? "selected='true' " : "");
				}
				if ($value != null){
					$selected = ($data ['codigo'] === $value ? "selected='true' " : "");
				}
				$html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data ['codigo'] . ">  " . simec_htmlentities ( $data ['descricao'] ) . " </option> ";
			}
		}
	
		$html .= '</select>';
	
		return $html;
	}
		
	public function getUf(){
		$sql = 'SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estuf';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'estuf');
	}	

    public function getEtapas($funcao = null){
        $sql = "select eieid as codigo, eiedsc as descricao from sase.etapaindeducacional where eiestatus = 'A' order by eiedsc";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        $valor = $_POST['eieid'] ? $_POST['eieid'] : 1;
        return $this->getOptions( $dados, array(), 'eieid', $funcao, $valor);
    }

	/**
	 * Retorna uma lista com as informações da Indicação dos Avaliadores Educacional
	 * 
	 * @param bool $form - Informa se a tabela apresentará campos editáveis 
	 */
	public function listar($form = false){
        $iaediretoria = $this->arAtributos['iaediretoria'] != '' ? $this->arAtributos['iaediretoria'] : $_REQUEST['iaediretoria'];
        $whereEie = " AND iae.iaeetapa = 1 ";
        if($_POST['eieid']){
            $whereEie = " AND iae.iaeetapa = {$_POST['eieid']} ";
        }
		$sql = "
				SELECT
					iae.iaeid,
					est.estuf,
					est.regcod,
					reg.regdescricao,
					(SELECT COUNT(*) FROM territorios.estado WHERE regcod = reg.regcod) AS countreg,
					(SELECT COUNT(*) FROM territorios.municipio WHERE estuf = est.estuf) AS iaemunicipiototal,
					iae.iaemunicipioatendido,
					iae.iaetecnicoprevisto,
					iae.iaetecnicoindicado,
					iae.iaesupervisorprevisto,
					iae.iaesupervisorindicado,
					iae.iaeexecutivoprevisto,
					iae.iaeexecutivoindicado,
					iae.iaesupervgeralprevisto,
					iae.iaesupervgeralindicado,
					iae.iaestatus
				FROM territorios.estado est
				INNER JOIN territorios.regiao reg ON (est.regcod = reg.regcod)
				LEFT JOIN sase.indavaleducacional iae ON (iae.estuf = est.estuf) {$whereEie} AND iaediretoria = {$iaediretoria}
				ORDER BY est.regcod, est.estuf
				";
		
		$dados = $this->carregar($sql);
		
		if ($dados){
            $hideTecnico             = false;
            $hideSupervisor          = false;
            $hideSupervisorGeral     = false;
            $hideCoordenadorEstadual = false;
            switch($iaediretoria){
                case 2:
                    $hideSupervisor          = true;
                    $hideSupervisorGeral     = true;
                    break;

            }
			?>
            <style>
                .hideTecnico{display: none;}
                .hideSupervisor{display: none;}
                .hideSupervisorGeral{display: none;}
                .hideCoordenadorEstadual{display: none;}
            </style>
			<div id="listagem" style="background: #ffffff !important;">
				<table class="table table-bordered table-hover table-condensed" border="1">
					<thead>
						<tr>
							<th rowspan="2">UF</th>
							<th colspan="3">Município</th>
							<th colspan="3" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>AEs Técnico</th>
							<th colspan="3" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>AEs Supervisores</th>
							<th colspan="3" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>AEs Supervisor Geral</th>
							<th colspan="3" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>AEs Coordenadores Estadual</th>
							<th colspan="3">Total</th>
						</tr>
						<tr>
							<th>Total</th>
							<th>Atendido</th>
							<th>%</th>
							<th <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>Previsto</th>
							<th <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>Indicado</th>
							<th <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>Pendente</th>
							<th <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>Previsto</th>
							<th <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>Indicado</th>
							<th <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>Pendente</th>
							<th <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>Previsto</th>
							<th <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>Indicado</th>
							<th <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>Pendente</th>
							<th <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>Previsto</th>
							<th <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>Indicado</th>
							<th <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>Pendente</th>
							<th>Previsto</th>
							<th>Indicado</th>
							<th>Pendente</th>
						</tr>
					</thead>
					<tbody>
						<?php
							$p = "6.25%";
							$w = "60px";
							$i = 1;
							$c = 1;
							
							$regCod = 0;
							
							$totalMun = 0;
							$totalMunAtendido = 0;
							$totalMunPercent  = 0;
							
							$totalTecPrevisto = 0;
							$totalTecIndicado = 0;
							$totalTecPercent  = 0;
							
							$totalSupPrevisto = 0;
							$totalSupIndicado = 0;
							$totalSupPercent  = 0;

							$totalExePrevisto = 0;
							$totalExeIndicado = 0;
							$totalExePercent  = 0;

							$totalSupGeralPrevisto = 0;
							$totalSupGeralIndicado = 0;
							
							$totalM = 0;
							$totalMA = 0;
							$totalMP  = 0;
								
							$totalTeP = 0;
							$totalTeI = 0;
								
							$totalSP = 0;
							$totalSI = 0;
							
							$totalEP = 0;
							$totalEI = 0;

							$totalSGP = 0;
							$totalSGI = 0;
															
							$totalTP = 0;
							$totalTI = 0;
							$totalPE = 0;
							
							if ($dados) {
                                foreach ($dados as $dado) {
                                    if ($regCod == 0) {
                                        $regCod = $dado['regcod'];
                                        $regDsc = $dado['regdescricao'];
                                    }

                                    ?>
                                    <tr style="background-color: <?php echo $this->arCoresRegiao[0][trim($dado['regcod'])] ?> !important;">
                                        <td style="text-align: center;" width="<?php echo $p ?>">
                                            <input type="hidden" id="iaeid[<?php echo $i ?>]"
                                                   name="iaeid[<?php echo $i ?>]" value="<?php echo $dado['iaeid'] ?>"/>
                                            <input type="hidden" id="estuf[<?php echo $i ?>]"
                                                   name="estuf[<?php echo $i ?>]" value="<?php echo $dado['estuf'] ?>"/>
                                            <?php echo $dado['estuf'] ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>">
                                            <?php echo $dado['iaemunicipiototal'] ?>
                                        </td>
                                        <td style="text-align: center;" align="center" width="<?php echo $p ?>">
                                            <?php if ($form) { ?>
                                                <input name="iaemunicipioatendido[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaemunicipioatendido'] ?>">
                                            <?php } else {
                                                echo $dado['iaemunicipioatendido'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>">
                                            <?php
                                            echo round(($dado['iaemunicipioatendido'] * 100) / $dado['iaemunicipiototal']);
                                            ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaetecnicoprevisto[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaetecnicoprevisto'] ?>">
                                            <?php } else {
                                                echo $dado['iaetecnicoprevisto'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaetecnicoindicado[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaetecnicoindicado'] ?>">
                                            <?php } else {
                                                echo $dado['iaetecnicoindicado'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>>
                                            <?php
                                            echo $dado['iaetecnicoprevisto'] - $dado['iaetecnicoindicado'];
                                            ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaesupervisorprevisto[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaesupervisorprevisto'] ?>">
                                            <?php } else {
                                                echo $dado['iaesupervisorprevisto'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaesupervisorindicado[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaesupervisorindicado'] ?>">
                                            <?php } else {
                                                echo $dado['iaesupervisorindicado'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>>
                                            <?php
                                            echo $dado['iaesupervisorprevisto'] - $dado['iaesupervisorindicado'];
                                            ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaeexecutivoprevisto[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaeexecutivoprevisto'] ?>">
                                            <?php } else {
                                                echo $dado['iaeexecutivoprevisto'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaeexecutivoindicado[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaeexecutivoindicado'] ?>">
                                            <?php } else {
                                                echo $dado['iaeexecutivoindicado'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>>
                                            <?php
                                            echo $dado['iaeexecutivoprevisto'] - $dado['iaeexecutivoindicado'];
                                            ?>
                                        </td>

                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaesupervgeralprevisto[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaesupervgeralprevisto'] ?>">
                                            <?php } else {
                                                echo $dado['iaesupervgeralprevisto'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>
                                            <?php if ($form) { ?>
                                                <input name="iaesupervgeralindicado[<?php echo $i ?>]" type="text"
                                                       class="form-control campoNumGrid" maxlength="4"
                                                       style="width: <?php echo $w ?>;"
                                                       value="<?php echo $dado['iaesupervgeralindicado'] ?>">
                                            <?php } else {
                                                echo $dado['iaesupervgeralindicado'];
                                            } ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>>
                                            <?php
                                            echo $dado['iaesupervgeralprevisto'] - $dado['iaesupervgeralindicado'];
                                            ?>
                                        </td>

                                        <td style="text-align: center;" width="<?php echo $p ?>">
                                            <?php
                                            $tp = $dado['iaetecnicoprevisto'] + $dado['iaesupervisorprevisto'] + $dado['iaeexecutivoprevisto'] + $dado['iaesupervgeralprevisto'];
                                            $totalTP += $tp;
                                            echo $tp;
                                            ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>">
                                            <?php
                                            $ti = $dado['iaetecnicoindicado'] + $dado['iaesupervisorindicado'] + $dado['iaeexecutivoindicado'] + $dado['iaesupervgeralindicado'];
                                            $totalTI += $ti;
                                            echo $ti;
                                            ?>
                                        </td>
                                        <td style="text-align: center;" width="<?php echo $p ?>">
                                            <?php
                                            $pe = $tp - $ti;
                                            $totalPE = $pe;
                                            echo $pe;
                                            ?>
                                        </td>
                                    </tr>
                                    <?php
                                    if ($c == $dado['countreg']) {
                                        $c = 0;
                                        $regCod = $dado['regcod'];
                                        $regDsc = $dado['regdescricao'];

                                        $totalMun += $dado['iaemunicipiototal'];
                                        $totalMunAtendido += $dado['iaemunicipioatendido'];
                                        $totalMunPercent = round(($totalMunAtendido * 100) / $totalMun);

                                        $totalTecPrevisto += $dado['iaetecnicoprevisto'];
                                        $totalTecIndicado += $dado['iaetecnicoindicado'];

                                        $totalSupPrevisto += $dado['iaesupervisorprevisto'];
                                        $totalSupIndicado += $dado['iaesupervisorindicado'];

                                        $totalExePrevisto += $dado['iaeexecutivoprevisto'];
                                        $totalExeIndicado += $dado['iaeexecutivoindicado'];

                                        $totalSupGeralPrevisto += $dado['iaesupervgeralprevisto'];
                                        $totalSupGeralIndicado += $dado['iaesupervgeralindicado'];

                                        $totalM += $totalMun;
                                        $totalMA += $totalMunAtendido;
                                        $totalMP = round(($totalMA * 100) / $totalM);;

                                        $totalTeP += $totalTecPrevisto;
                                        $totalTeI += $totalTecIndicado;

                                        $totalSP += $totalSupPrevisto;
                                        $totalSI += $totalSupIndicado;

                                        $totalEP += $totalExePrevisto;
                                        $totalEI += $totalExeIndicado;

                                        $totalSGP += $totalSupGeralPrevisto;
                                        $totalSGI += $totalSupGeralIndicado;

                                        ?>
                                        <tr style="background-color: <?php echo $this->arCoresRegiao[1][trim($dado['regcod'])] ?> !important; font-weight: bold;">
                                            <td style="text-align: center;"><?php echo $regDsc ?></td>
                                            <td style="text-align: center;"><?php echo $totalMun ?></td>
                                            <td style="text-align: center;"><?php echo $totalMunAtendido ?></td>
                                            <td style="text-align: center;"><?php echo $totalMunPercent ?></td>

                                            <td style="text-align: center;" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>><?php echo $totalTecPrevisto ?></td>
                                            <td style="text-align: center;" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>><?php echo $totalTecIndicado ?></td>
                                            <td style="text-align: center;" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>><?php echo($totalTecPrevisto - $totalTecIndicado) ?></td>

                                            <td style="text-align: center;" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>><?php echo $totalSupPrevisto ?></td>
                                            <td style="text-align: center;" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>><?php echo $totalSupIndicado ?></td>
                                            <td style="text-align: center;" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>><?php echo($totalSupPrevisto - $totalSupIndicado) ?></td>

                                            <td style="text-align: center;" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>><?php echo $totalExePrevisto ?></td>
                                            <td style="text-align: center;" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>><?php echo $totalExeIndicado ?></td>
                                            <td style="text-align: center;" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>><?php echo($totalExePrevisto - $totalExeIndicado) ?></td>

                                            <td style="text-align: center;" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>><?php echo $totalSupGeralPrevisto ?></td>
                                            <td style="text-align: center;" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>><?php echo $totalSupGeralIndicado ?></td>
                                            <td style="text-align: center;" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>><?php echo($totalSupGeralPrevisto - $totalSupGeralIndicado) ?></td>

                                            <td style="text-align: center;"><?php echo $totalTP ?></td>
                                            <td style="text-align: center;"><?php echo $totalTI ?></td>
                                            <td style="text-align: center;"><?php echo($totalTP - $totalTI) ?></td>
                                        </tr>
                                        <?php

                                        $totalMun = 0;
                                        $totalMunAtendido = 0;
                                        $totalMunPercent = 0;

                                        $totalTecPrevisto = 0;
                                        $totalTecIndicado = 0;
                                        $totalTecPercent = 0;

                                        $totalSupPrevisto = 0;
                                        $totalSupIndicado = 0;
                                        $totalSupPercent = 0;

                                        $totalExePrevisto = 0;
                                        $totalExeIndicado = 0;
                                        $totalExePercent = 0;

                                        $totalSupGeralPrevisto = 0;
                                        $totalSupGeralIndicado = 0;

                                        $totalTP = 0;
                                        $totalTI = 0;
                                        $totalPE = 0;
                                    } else {
                                        $totalMun += $dado['iaemunicipiototal'];
                                        $totalMunAtendido += $dado['iaemunicipioatendido'];
                                        $totalMunPercent = round(($totalMunAtendido * 100) / $totalMun);

                                        $totalTecPrevisto += $dado['iaetecnicoprevisto'];
                                        $totalTecIndicado += $dado['iaetecnicoindicado'];

                                        $totalSupPrevisto += $dado['iaesupervisorprevisto'];
                                        $totalSupIndicado += $dado['iaesupervisorindicado'];

                                        $totalExePrevisto += $dado['iaeexecutivoprevisto'];
                                        $totalExeIndicado += $dado['iaeexecutivoindicado'];

                                        $totalSupGeralPrevisto += $dado['iaesupervgeralprevisto'];
                                        $totalSupGeralIndicado += $dado['iaesupervgeralindicado'];
                                    }

                                    $i++;
                                    $c++;
                                }
                            }
							?>
						<tr style="background-color: #FFFFFF !important; font-weight: bold;">
							<td style="text-align: center;">Total</td>
							<td style="text-align: center;"><?php echo $totalM ?></td>
							<td style="text-align: center;"><?php echo $totalMA ?></td>
							<td style="text-align: center;"><?php echo $totalMP ?></td>
							
							<td style="text-align: center;" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>><?php echo $totalTeP ?></td>
							<td style="text-align: center;" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>><?php echo $totalTeI ?></td>
							<td style="text-align: center;" <?= $hideTecnico ? 'class="hideTecnico"' : '' ?>><?php echo ($totalTeP - $totalTeI) ?></td>
							
							<td style="text-align: center;" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>><?php echo $totalSP ?></td>
							<td style="text-align: center;" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>><?php echo $totalSI ?></td>
							<td style="text-align: center;" <?= $hideSupervisor ? 'class="hideSupervisor"' : '' ?>><?php echo ($totalSP - $totalSI) ?></td>
							
							<td style="text-align: center;" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>><?php echo $totalEP ?></td>
							<td style="text-align: center;" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>><?php echo $totalEI ?></td>
							<td style="text-align: center;" <?= $hideSupervisorGeral ? 'class="hideSupervisorGeral"' : '' ?>><?php echo ($totalEP - $totalEI) ?></td>

							<td style="text-align: center;" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>><?php echo $totalSGP ?></td>
							<td style="text-align: center;" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>><?php echo $totalSGI ?></td>
							<td style="text-align: center;" <?= $hideCoordenadorEstadual ? 'class="hideCoordenadorEstadual"' : '' ?>><?php echo ($totalSGP - $totalSGI) ?></td>
														
							<td style="text-align: center;"><?php echo ($totalTeP + $totalSP + $totalEP + $totalSGP) ?></td>
							<td style="text-align: center;"><?php echo ($totalTeI + $totalSI + $totalEI + $totalSGI) ?></td>
							<td style="text-align: center;"><?php echo ($totalTeP + $totalSP + $totalEP + $totalSGP) - ($totalTeI + $totalSI + $totalEI + $totalSGI) ?></td>
						</tr>
					</tbody>
				</table>
			</div>
			<script>
				$(document).ready(function(){
					$('.campoNumGrid').mask("9999");
				});
			</script>
			<?php
		}
	}
	
}