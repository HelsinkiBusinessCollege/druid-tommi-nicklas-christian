<?php
/**
 * @file
 *
 * Rules integration for commerce checkout by Amazon IOPN module.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_cba_rules_event_info() {
  $info = array();

  $info['commerce_cba_iopn_new_order_notification'] = array(
    'group' => t('Commerce cba'),
    'label' => t('New Order notification received from Amazon'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
      ),
      'shipping_address' => array(
        'type' => 'struct',
        'label' => t('Shipping address returned by Amazon'),
      ),
      'buyer_info' => array(
        'type' => 'struct',
        'label' => t('Buyer info returned by Amazon'),
        'description' => t('Normally buyer name and email address'),
      ),
    ),
  );

  $info['commerce_cba_iopn_rts_order_notification'] = array(
    'group' => t('Commerce cba'),
    'label' => t('Ready to ship Order notification received from Amazon'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
      ),
      'shipping_address' => array(
        'type' => 'struct',
        'label' => t('Shipping address returned by Amazon'),
      ),
      'billing_address' => array(
        'type' => 'struct',
        'label' => t('Shipping address returned by Amazon'),
      ),
      'buyer_info' => array(
        'type' => 'struct',
        'label' => t('Buyer info returned by Amazon'),
        'description' => t('Normally buyer name and email address'),
      ),
    ),
  );

  $info['commerce_cba_iopn_cancel_order_notification'] = array(
    'group' => t('Commerce cba'),
    'label' => t('Cancel Order notification received from Amazon'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
      ),
      'shipping_address' => array(
        'type' => 'struct',
        'label' => t('Shipping address returned by Amazon'),
      ),
      'buyer_info' => array(
        'type' => 'struct',
        'label' => t('Buyer info returned by Amazon'),
        'description' => t('Normally buyer name and email address'),
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_cba_rules_action_info(){
  $info = array();

  $info['commerce_cba_iopn_save_customer_profile'] = array(
    'group' => t('Commerce cba'),
    'base' => 'commerce_cba_save_customer_profile_from_amazon_address',
    'label' => t('Create/alter the customer profile from the address returned by Amazon'),
    'description' => t('This will override the customer profile associated to the order if any.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
        'save' => TRUE,
      ),
      'customer_profile_field' => array(
        'type' => 'commerce_cba_iopn_select_profile_field',
        'label' => t('Customer profile type'),
        'restriction' => 'input',
      ),
      'amazon_address' => array(
        'type' => 'struct',
        'label' => t('Address data returned by Amazon'),
      ),
    ),
    'provides' => array(
      'commerce_customer_profile' => array(
        'type' => 'commerce_customer_profile',
        'label' => t('Cusomer profile saved'),
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_rules_data_info().
 */
function commerce_cba_rules_data_info() {
  $data['commerce_cba_iopn_select_profile_field'] = array(
    'label' => t('Select customer profile'),
    'ui class' => 'RulesDataUICommerceCbaIopnProfile',
  );
  return $data;
}

/**
 * Adds a customer field selection.
 */
class RulesDataUICommerceCbaIopnProfile extends RulesDataUI implements RulesDataDirectInputFormInterface {

  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $profile_types = commerce_customer_profile_types();
    $options = array();
    foreach ($profile_types as $id => $type) {
      if ($type['addressfield'] == TRUE) {
        $options[$id] = $type['name'];
      }
    }

    $form[$name] = array(
      '#type' => 'select',
      '#title' => t('Select the customer profile field to store the address'),
      '#options' => $options,
    );
    if (!empty($settings) && !empty($settings['customer_profile_field'])) {
      $form[$name]['#default_value'] = $settings['customer_profile_field'];
    }

    return $form;
  }

  public static function render($value) {
    return array();
  }

}
